import os
import isa.defs
from isa.codegen import generator
from isa.core.registry import INSTRUCTIONS
from isa.core.enums    import FMT_PSEUDO
from isa.core.operands import RegisterOperand, ImmediateOperand, LabelOperand

def _union_field(op) -> str:
    if isinstance(op, RegisterOperand): return "reg"
    if isinstance(op, ImmediateOperand): return "imm"
    if isinstance(op, LabelOperand):    return "label"
    raise ValueError(f"неизвестный тип операнда: {type(op)}")
 
def _operand_kind(op) -> str:
    if isinstance(op, RegisterOperand): return "ASM_OP_REG"
    if isinstance(op, ImmediateOperand): return "ASM_OP_IMM"
    if isinstance(op, LabelOperand):    return "ASM_OP_LABEL"
    raise ValueError(f"неизвестный тип операнда: {type(op)}")

def gen_operands_header() -> str:
    return """\
/* Автогенерировано. Не редактировать вручную. */
#pragma once
#include <stdint.h>
 
typedef enum {
    ASM_OP_REG   = 0,   /* номер регистра 0..31         */
    ASM_OP_IMM   = 1,   /* числовая константа           */
    ASM_OP_LABEL = 2,   /* символьная метка (строка)    */
} asm_operand_type_t;
 
typedef struct {
    asm_operand_type_t type;
    union {
        uint32_t    reg;    /* ASM_OP_REG   */
        int64_t     imm;    /* ASM_OP_IMM   */
        const char *label;  /* ASM_OP_LABEL */
    };
} asm_operand_t;
"""

def _gen_wrapper(ins) -> list[str]:
    """
    Генерирует:
 
      static uint32_t asm_encode_add(asm_operand_t *ops) {
          return encode_add(ops[0].reg, ops[1].reg, ops[2].reg);
      }
    """
    lines = [
        f"static uint32_t asm_encode_{ins.mnemonic}(asm_operand_t *ops) {{",
    ]
 
    # Для инструкций без операндов ops не используется — подавляем warning
    if not ins.operands:
        lines.append("    (void)ops;")
 
    args = []
    for i, op in enumerate(ins.operands):
        field = _union_field(op)
 
        # Кастуем к нужному типу при передаче в encode_*
        if isinstance(op, RegisterOperand):
            args.append(f"(uint8_t)ops[{i}].{field}")
        elif isinstance(op, ImmediateOperand):
            if op.signed:
                ctype = "int16_t" if op.bits <= 16 else "int32_t"
            else:
                ctype = "uint16_t" if op.bits <= 16 else "uint32_t"
            if op.bits == 5:
                ctype = "uint8_t"
            args.append(f"({ctype})ops[{i}].{field}")
        elif isinstance(op, LabelOperand):
            # label к моменту encode уже разрешён в imm ассемблером
            args.append(f"(int16_t)ops[{i}].imm")
 
    joined = ", ".join(args)
    lines.append(f"    return encode_{ins.mnemonic}({joined});")
    lines.append("}")
    return lines

def gen_wrappers_header() -> str:
    lines = [
        "/* Автогенерировано. Не редактировать вручную. */",
        "#pragma once",
        "#include <stdint.h>",
        '#include "asm_operands.h"',
        '#include "codec.h"',
        "",
    ]
 
    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO:
            continue
        lines += _gen_wrapper(ins)
        lines.append("")
 
    return "\n".join(lines)

def _gen_operand_sig(ins) -> str:
    """
    Генерирует массив типов операндов для валидации:
      { ASM_OP_REG, ASM_OP_REG, ASM_OP_REG }
    """
    if not ins.operands:
        return "{}"
    kinds = ", ".join(_operand_kind(op) for op in ins.operands)
    return f"{{ {kinds} }}"
 
 
def gen_table_header() -> str:
    real_instrs = [i for i in INSTRUCTIONS if i.fmt != FMT_PSEUDO]
 
    lines = [
        "/* Автогенерировано. Не редактировать вручную. */",
        "#pragma once",
        "#include <stdint.h>",
        "#include <string.h>",
        '#include "asm_operands.h"',
        '#include "asm_wrappers.h"',
        "",
        "#define ASM_MAX_OPERANDS 3",
        "",
        "typedef struct {",
        "    const char            *mnemonic;",
        "    uint8_t                operand_count;",
        "    asm_operand_type_t     operand_types[ASM_MAX_OPERANDS];",
        "    uint32_t             (*encode)(asm_operand_t *ops);",
        "} asm_instr_t;",
        "",
        "static const asm_instr_t ASM_TABLE[] = {",
    ]
 
    for ins in real_instrs:
        count  = len(ins.operands)
        sig    = _gen_operand_sig(ins)
        # Дополняем до ASM_MAX_OPERANDS нулями если нужно
        if ins.operands:
            kinds  = [_operand_kind(op) for op in ins.operands]
            pad    = ["0"] * (3 - len(kinds))
            sig    = "{ " + ", ".join(kinds + pad) + " }"
        else:
            sig = "{ 0, 0, 0 }"
 
        lines.append(
            f'    {{ "{ins.mnemonic}", {count}, {sig}, asm_encode_{ins.mnemonic} }},'
        )
 
    lines += [
        "};",
        "",
        f"#define ASM_TABLE_SIZE {len(real_instrs)}",
        "",
        "static inline const asm_instr_t *lookup_mnemonic(const char *mnemonic) {{",
        "    for (int i = 0; i < ASM_TABLE_SIZE; i++) {",
        "        if (strcmp(ASM_TABLE[i].mnemonic, mnemonic) == 0)",
        "            return &ASM_TABLE[i];",
        "    }",
        "    return NULL;",
        "}",
        "",
        "static inline int validate_operands(const asm_instr_t *desc, asm_operand_t *ops, int count) {{",
        "    if (count != desc->operand_count)",
        "        return 0;",
        "    for (int i = 0; i < count; i++) {",
        "        /* LABEL совместим с IMM — ассемблер разрешает метку в imm */",
        "        asm_operand_type_t expected = desc->operand_types[i];",
        "        asm_operand_type_t actual   = ops[i].type;",
        "        if (actual == ASM_OP_LABEL && expected == ASM_OP_IMM)",
        "            continue;",
        "        if (actual != expected)",
        "            return 0;",
        "    }",
        "    return 1;",
        "}",
    ]
 
    return "\n".join(lines) + "\n"

@generator("c_asm")
def run(out_dir: str) -> None:
    files = {
        "asm_operands.h": gen_operands_header(),
        "asm_wrappers.h": gen_wrappers_header(),
        "asm_table.h":    gen_table_header(),
    }
    for filename, content in files.items():
        path = os.path.join(out_dir, filename)
        with open(path, "w") as f:
            f.write(content)
        print(f"[c_asm] written to {path}")
