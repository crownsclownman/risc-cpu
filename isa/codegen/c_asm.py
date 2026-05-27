import os
from isa.codegen import generator
from isa.core.registry import INSTRUCTIONS
from isa.core.enums    import FMT_PSEUDO
from isa.core.operands import RegisterOperand, ImmediateOperand, LabelOperand, OperandKind

KIND_TO_C = {
    OperandKind.REG:     "ASM_SIG_REG",
    OperandKind.IMM16S:  "ASM_SIG_IMM16S",
    OperandKind.IMM16U:  "ASM_SIG_IMM16U",
    OperandKind.IMM26U:  "ASM_SIG_IMM26U",
    OperandKind.SHAMT:   "ASM_SIG_SHAMT",
    OperandKind.CSR:     "ASM_SIG_CSR",
    OperandKind.BRANCH:  "ASM_SIG_BRANCH",
    OperandKind.JTARGET: "ASM_SIG_JTARGET",
    OperandKind.LABEL:   "ASM_SIG_LABEL",
}
KIND_RANGE = {
    OperandKind.IMM16S:  (-32768, 32767),
    OperandKind.IMM16U:  (0, 65535),
    OperandKind.IMM26U:  (0, 0x3FFFFFF),
    OperandKind.SHAMT:   (0, 31),
    OperandKind.CSR:     (0, 31),
    OperandKind.BRANCH:  (-32768, 32767),
    OperandKind.JTARGET: (0, 0x3FFFFFF),
}
KIND_UNION_FIELD = {
    OperandKind.REG: "reg", OperandKind.IMM16S: "imm",
    OperandKind.IMM16U: "imm", OperandKind.IMM26U: "imm",
    OperandKind.SHAMT: "imm", OperandKind.CSR: "imm",
    OperandKind.BRANCH: "imm", OperandKind.JTARGET: "imm",
    OperandKind.LABEL: "label",
}
KIND_CAST = {
    OperandKind.REG: "uint8_t", OperandKind.IMM16S: "int16_t",
    OperandKind.IMM16U: "uint16_t", OperandKind.IMM26U: "uint32_t",
    OperandKind.SHAMT: "uint8_t", OperandKind.CSR: "uint8_t",
    OperandKind.BRANCH: "int16_t", OperandKind.JTARGET: "uint32_t",
    OperandKind.LABEL: "int16_t",
}

def _op_kind(op):
    if isinstance(op, RegisterOperand): return OperandKind.REG
    if isinstance(op, LabelOperand):    return OperandKind.LABEL
    return op.kind

def gen_operands_header():
    used = {_op_kind(op) for ins in INSTRUCTIONS if ins.fmt != FMT_PSEUDO
            for op in ins.operands}
    L = ["/* Автогенерировано. */", "#pragma once", "#include <stdint.h>", "",
         "typedef enum {"]
    for k, c in KIND_TO_C.items():
        L.append(f"    {c},{' /* используется */' if k in used else ''}")
    L += ["} asm_operand_sig_t;", "",
          "typedef enum {", "    ASM_OP_REG=0, ASM_OP_IMM=1, ASM_OP_LABEL=2,",
          "} asm_operand_type_t;", "",
          "#define ASM_MAX_OPERANDS 8", "",
          "typedef struct {", "    asm_operand_type_t type;",
          "    union { uint32_t reg; int64_t imm; const char *label; };",
          "} asm_operand_t;"]
    return "\n".join(L) + "\n"

def _gen_wrapper(ins):
    L = [f"static uint32_t asm_encode_{ins.mnemonic}(asm_operand_t *ops) {{"]
    if not ins.operands:
        L.append("    (void)ops;")

    args = [
        f"({KIND_CAST[_op_kind(op)]})ops[{i}].{KIND_UNION_FIELD[_op_kind(op)]}"
        for i, op in enumerate(ins.operands)
    ]

    L.append(f"    return encode_{ins.mnemonic}({', '.join(args)});")
    L.append("}")
    return L

def gen_wrappers_header():
    L = ["/* Автогенерировано. */", "#pragma once", "#include <stdint.h>",
         '#include "asm_operands.h"', '#include "codec.h"', ""]
    for ins in INSTRUCTIONS:
        if ins.fmt != FMT_PSEUDO:
            L += _gen_wrapper(ins)
            L.append("")
    return "\n".join(L)

def _sig_array(ins):
    kinds = [KIND_TO_C[_op_kind(op)] for op in ins.operands]
    return "{ " + ", ".join(kinds + ["0"] * (8 - len(kinds))) + " }"

def gen_table_header():
    real = [i for i in INSTRUCTIONS if i.fmt != FMT_PSEUDO]
    L = ["/* Автогенерировано. */", "#pragma once",
         "#include <stdint.h>", "#include <string.h>",
         '#include "asm_operands.h"', '#include "asm_wrappers.h"', "",
         "typedef struct {",
         "    const char        *mnemonic;",
         "    uint8_t            operand_count;",
         "    asm_operand_sig_t  operand_sigs[ASM_MAX_OPERANDS];",
         "    uint32_t         (*encode)(asm_operand_t *ops);",
         "} asm_instr_t;", "",
         "static const asm_instr_t ASM_TABLE[] = {"]
    for ins in real:
        L.append(f'    {{ "{ins.mnemonic}", {len(ins.operands)}, {_sig_array(ins)}, asm_encode_{ins.mnemonic} }},')
    L += ["};", "", f"#define ASM_TABLE_SIZE {len(real)}", "",
          "static inline const asm_instr_t *lookup_mnemonic(const char *mnemonic) {",
          "    for (int i = 0; i < ASM_TABLE_SIZE; i++)",
          "        if (strcmp(ASM_TABLE[i].mnemonic, mnemonic) == 0)",
          "            return &ASM_TABLE[i];",
          "    return NULL;",
          "}", "",
          "static inline int validate_operands(const asm_instr_t *desc, asm_operand_t *ops, int count, const char **err_out) {",
          "    if (count != (int)desc->operand_count) {",
          '        if (err_out) *err_out = "wrong operand count";',
          "        return 0;",
          "    }",
          "    for (int i = 0; i < count; i++) {",
          "        asm_operand_sig_t  sig    = desc->operand_sigs[i];",
          "        asm_operand_type_t actual = ops[i].type;",
          "        if (sig == ASM_SIG_REG) {",
          "            if (actual != ASM_OP_REG) { if (err_out) *err_out = \"expected register\"; return 0; }",
          "            if (ops[i].reg > 31)       { if (err_out) *err_out = \"register out of range\"; return 0; }",
          "            continue;",
          "        }",
          "        if (sig == ASM_SIG_BRANCH || sig == ASM_SIG_JTARGET) {",
          "            if (actual == ASM_OP_LABEL) continue;",
          "            if (actual != ASM_OP_IMM) { if (err_out) *err_out = \"expected label or imm\"; return 0; }",
          "        } else if (actual != ASM_OP_IMM) {",
          "            if (err_out) *err_out = \"expected immediate\"; return 0;",
          "        }"]
    for kind, (lo, hi) in KIND_RANGE.items():
        c = KIND_TO_C[kind]
        L += [f"        if (sig == {c} && (ops[i].imm < {lo}LL || ops[i].imm > {hi}LL)) {{",
              f'            if (err_out) *err_out = "immediate out of range ({lo}..{hi})";',
              f"            return 0;",
              f"        }}"]
    L += ["    }", "    return 1;", "}"]
    return "\n".join(L) + "\n"

@generator("c_asm")
def run(out_dir):
    for fname, content in [
        ("asm_operands.h", gen_operands_header()),
        ("asm_wrappers.h", gen_wrappers_header()),
        ("asm_table.h",    gen_table_header()),
    ]:
        p = os.path.join(out_dir, fname)
        open(p, "w").write(content)
        print(f"[c_asm] written to {p}")
