"""
Генерирует из реестра инструкций:
  - enum OP   { ... }   — все уникальные опкоды
  - enum FUNCT { ... }  — все уникальные funct (только R-type)
  - instr_table.h       — таблица для дизассемблера / валидатора

Запуск:
    python -m isa.codegen.c_enums
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", ".."))

import isa.defs
from isa.core.registry import INSTRUCTIONS
from isa.core.enums    import FMT_R, FMT_PSEUDO

def _mnemonic_to_c(m: str) -> str:
    """addi → ADDI,  tlbinv → TLBINV"""
    return m.upper()

def _op_name(instr) -> str:
    return f"OP_{_mnemonic_to_c(instr.mnemonic)}"

def _funct_name(instr) -> str:
    return f"FUNCT_{_mnemonic_to_c(instr.mnemonic)}"

def gen_op_enum() -> str:
    lines = ["typedef enum {"]

    lines.append("    OP_SPECIAL = 0x00,  /* все R-type */")

    seen_opcodes = {0x00}  # SPECIAL уже добавлен

    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO:
            continue
        if ins.fmt == FMT_R:
            continue            # R-type всегда opcode=0x00
        if ins.opcode is None:
            continue
        if ins.opcode in seen_opcodes:
            continue

        seen_opcodes.add(ins.opcode)
        lines.append(f"    {_op_name(ins)} = 0x{ins.opcode:02X},")

    lines.append("} op_t;")
    return "\n".join(lines)

def gen_funct_enum() -> str:
    lines = ["typedef enum {"]

    seen_functs = set()

    for ins in INSTRUCTIONS:
        if ins.fmt != FMT_R:
            continue
        if ins.funct is None:
            continue
        if ins.funct in seen_functs:
            continue

        seen_functs.add(ins.funct)
        lines.append(f"    {_funct_name(ins)} = 0x{ins.funct:02X},")

    lines.append("} funct_t;")
    return "\n".join(lines)

def gen_instr_table() -> str:
    lines = [
        "/* Автогенерировано. Не редактировать вручную. */",
        "",
        "typedef struct {",
        "    const char *mnemonic;",
        "    uint8_t     opcode;",
        "    uint8_t     funct;",
        "    uint8_t     fmt;      /* 'R' / 'I' / 'J' */",
        "} instr_desc_t;",
        "",
        "static const instr_desc_t INSTR_TABLE[] = {",
    ]

    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO:
            continue

        opcode = ins.opcode if ins.opcode is not None else 0
        funct  = ins.funct  if ins.funct  is not None else 0
        fmt_c  = ins.fmt[0]  # 'R', 'I', 'J'

        lines.append(
            f'    {{ "{ins.mnemonic}", '
            f'0x{opcode:02X}, '
            f'0x{funct:02X}, '
            f"'{fmt_c}' }},"
        )

    lines.append("};")
    lines.append(
        f"\n#define INSTR_TABLE_SIZE "
        f"{sum(1 for i in INSTRUCTIONS if i.fmt != FMT_PSEUDO)}"
    )
    return "\n".join(lines)

def gen_header() -> str:
    parts = [
        "/* Автогенерировано из isa/. Не редактировать вручную. */",
        "#pragma once",
        "#include <stdint.h>",
        "",
        gen_op_enum(),
        "",
        gen_funct_enum(),
        "",
        gen_instr_table(),
    ]
    return "\n".join(parts) + "\n"

if __name__ == "__main__":
    out = gen_header()

    if len(sys.argv) > 1:
        path = sys.argv[1]
        with open(path, "w") as f:
            f.write(out)
        print(f"[codegen] записано: {path}")
    else:
        print(out)
