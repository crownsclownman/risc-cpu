"""
Генерирует codec.h и codec.c — макросы, структуры,
функции encode/decode для каждого формата и каждой инструкции.

Запуск:
    python -m isa.codegen.codec                        # stdout
    python -m isa.codegen.codec --out-dir ../emulator  # пишет файлы
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", ".."))

import isa.defs
from isa.codegen import generator
from isa.core.registry  import INSTRUCTIONS
from isa.core.enums     import FMT_R, FMT_I, FMT_J, FMT_PSEUDO
from isa.core.formats   import R_FORMAT, I_FORMAT, J_FORMAT

FORMATS = {
    FMT_R: R_FORMAT,
    FMT_I: I_FORMAT,
    FMT_J: J_FORMAT,
}

def _mask(width: int) -> str:
    return f"0x{((1 << width) - 1):X}u"

def _collect_fields():
    """
    Возвращает:
      all_fields  — dict { field_name: [(fmt, offset, width), ...] }
      collisions  — поля которые встречаются в нескольких форматах
                    с разными offset/width
    """
    all_fields: dict[str, list] = {}

    for fmt, fields in FORMATS.items():
        for f in fields:
            entry = (fmt, f.offset, f.width)
            all_fields.setdefault(f.source, []).append(entry)

    collisions = {}
    for name, entries in all_fields.items():
        # уникальные (offset, width) пары
        unique = list({(o, w) for _, o, w in entries})
        if len(unique) > 1:
            collisions[name] = entries

    return all_fields, collisions

def gen_header() -> str:
    all_fields, collisions = _collect_fields()
    lines = []

    lines += [
        "/* Автогенерировано из isa/. Не редактировать вручную. */",
        "#pragma once",
        "#include <stdint.h>",
        "",
    ]

    if collisions:
        lines.append("/*")
        lines.append(" * ВНИМАНИЕ: следующие поля имеют разные позиции в разных форматах.")
        lines.append(" * Для них макросы генерируются с суффиксом _<FMT>.")
        lines.append(" *")
        for name, entries in collisions.items():
            for fmt, offset, width in entries:
                lines.append(f" *   {name:<10} fmt={fmt}  offset={offset}  width={width}")
        lines.append(" */")
        lines.append("")

    lines.append("")

    emitted = set()   # (name, offset, width) — не дублировать одинаковые

    for fmt, fields in FORMATS.items():
        lines.append(f"/* {fmt}-type */")
        for f in fields:
            key = (f.source, f.offset, f.width)
            mask = _mask(f.width)

            in_collision = f.source in collisions
            macro_base = f.source.upper()
            if in_collision:
                macro_base = f"{f.source.upper()}_{fmt}"

            if key not in emitted:
                lines.append(
                    f"#define FIELD_{macro_base}_SHIFT  {f.offset}u"
                )
                lines.append(
                    f"#define FIELD_{macro_base}_MASK   {mask}"
                )
                # Удобный экстрактор
                lines.append(
                    f"#define FIELD_{macro_base}_GET(w) "
                    f"(((w) >> {f.offset}u) & {mask})"
                )
                # Удобный упаковщик
                lines.append(
                    f"#define FIELD_{macro_base}_PUT(v) "
                    f"(((uint32_t)(v) & {mask}) << {f.offset}u)"
                )
                lines.append("")
                emitted.add(key)

        lines.append("")

    lines += [
        "typedef struct {",
        "    uint32_t raw;",
        "",
        "    /* Общие поля (присутствуют во всех форматах) */",
        "    uint8_t  opcode;",
        "",
        "    /* R-type */",
        "    uint8_t  rs;",
        "    uint8_t  rt;",
        "    uint8_t  rd;",
        "    uint8_t  shamt;",
        "    uint8_t  funct;",
        "",
        "    /* I-type */",
        "    uint16_t imm;",
        "",
        "    /* J-type */",
        "    uint32_t target;",
        "} decoded_instruction_t;",
        "",
    ]

    lines += [
        "void decode  (uint32_t instr, decoded_instruction_t *d);",
        "void decode_r(uint32_t instr, decoded_instruction_t *d);",
        "void decode_i(uint32_t instr, decoded_instruction_t *d);",
        "void decode_j(uint32_t instr, decoded_instruction_t *d);",
        "",
        "uint32_t encode_r(uint8_t rs, uint8_t rt, uint8_t rd,",
        "                  uint8_t shamt, uint8_t funct);",
        "uint32_t encode_i(uint8_t opcode, uint8_t rs, uint8_t rt,",
        "                  uint16_t imm);",
        "uint32_t encode_j_raw(uint8_t opcode, uint32_t target);",
        "",
    ]

    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO:
            continue
        proto = _encode_proto(ins, declaration=True)
        lines.append(proto + ";")

    lines.append("")
    return "\n".join(lines)


def gen_source() -> str:
    lines = []

    lines += [
        "/* Автогенерировано из isa/. Не редактировать вручную. */",
        '#include "codec.h"',
        '#include <stdint.h>',
        "",
    ]

    lines += _gen_decode_fmt(FMT_R, "decode_r")
    lines += _gen_decode_fmt(FMT_I, "decode_i")
    lines += _gen_decode_fmt(FMT_J, "decode_j")

    lines += _gen_decode_dispatch()

    lines += _gen_encode_fmt()

    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO:
            continue
        lines += _gen_encode_instr(ins)

    return "\n".join(lines)


def _gen_decode_fmt(fmt: str, fname: str) -> list[str]:
    fields = FORMATS[fmt]
    _, collisions = _collect_fields()
    lines = [f"void {fname}(uint32_t instr, decoded_instruction_t *d) {{"]
    lines.append("    d->raw = instr;")

    for f in fields:
        in_collision = f.source in collisions
        macro_base = f.source.upper()
        if in_collision:
            macro_base = f"{f.source.upper()}_{fmt}"

        field_struct = f.source   # имя поля в структуре (opcode, rs, rt...)
        lines.append(
            f"    d->{field_struct:<8} = FIELD_{macro_base}_GET(instr);"
        )

    lines.append("}")
    lines.append("")
    return lines


def _gen_decode_dispatch() -> list[str]:
    opcode_fmt: dict[int, str] = {}

    for ins in INSTRUCTIONS:
        if ins.fmt == FMT_PSEUDO or ins.opcode is None:
            continue
        if ins.opcode not in opcode_fmt:
            opcode_fmt[ins.opcode] = ins.fmt

    lines = [
        "void decode(uint32_t instr, decoded_instruction_t *d) {",
        "    uint8_t opcode = FIELD_OPCODE_GET(instr);",
        "",
        "    switch (opcode) {",
    ]

    for opcode in sorted(opcode_fmt):
        fmt = opcode_fmt[opcode]
        fn  = f"decode_{fmt.lower()}"
        lines.append(f"    case 0x{opcode:02X}: {fn}(instr, d); break;")

    lines += [
        "    default:",
        "        /* Неизвестный опкод — заполняем raw, opcode; остальное 0 */",
        "        d->raw    = instr;",
        "        d->opcode = opcode;",
        "        d->rs = d->rt = d->rd = d->shamt = d->funct = 0;",
        "        d->imm    = 0;",
        "        d->target = 0;",
        "        break;",
        "    }",
        "}",
        "",
    ]
    return lines


def _gen_encode_fmt() -> list[str]:
    _, collisions = _collect_fields()

    def macro(name, fmt):
        in_col = name in collisions
        base = name.upper() if not in_col else f"{name.upper()}_{fmt}"
        return f"FIELD_{base}_PUT"

    lines = [
        "uint32_t encode_r(uint8_t rs, uint8_t rt, uint8_t rd,",
        "                  uint8_t shamt, uint8_t funct) {",
        f"    return {macro('opcode', FMT_R)}(0x00u)",
        f"         | {macro('rs',     FMT_R)}(rs)",
        f"         | {macro('rt',     FMT_R)}(rt)",
        f"         | {macro('rd',     FMT_R)}(rd)",
        f"         | {macro('shamt',  FMT_R)}(shamt)",
        f"         | {macro('funct',  FMT_R)}(funct);",
        "}",
        "",
        "uint32_t encode_i(uint8_t opcode, uint8_t rs, uint8_t rt,",
        "                  uint16_t imm) {",
        f"    return {macro('opcode', FMT_I)}(opcode)",
        f"         | {macro('rs',     FMT_I)}(rs)",
        f"         | {macro('rt',     FMT_I)}(rt)",
        f"         | {macro('imm',    FMT_I)}(imm);",
        "}",
        "",
        "uint32_t encode_j_raw(uint8_t opcode, uint32_t target) {",
        f"    return {macro('opcode', FMT_J)}(opcode)",
        f"         | {macro('target', FMT_J)}(target);",
        "}",
        "",
    ]
    return lines


from isa.core.operands import RegisterOperand, ImmediateOperand, LabelOperand

def _encode_params(ins) -> list[tuple[str, str]]:
    """
    Возвращает список (тип_c, имя) параметров для encode_<mnemonic>.
    Псевдоинструкции пропускаем выше.
    """
    params = []
    for op in ins.operands:
        if isinstance(op, RegisterOperand):
            params.append(("uint8_t", op.name))
        elif isinstance(op, ImmediateOperand):
            ctype = "int16_t" if op.signed else "uint16_t"
            if op.bits == 5:
                ctype = "uint8_t"
            elif op.bits == 26:
                ctype = "uint32_t"
            params.append((ctype, op.name))
        elif isinstance(op, LabelOperand):
            params.append(("int16_t", op.name))   # смещение ветвления
    return params


def _encode_proto(ins, declaration=False) -> str:
    params = _encode_params(ins)
    param_str = ", ".join(f"{t} {n}" for t, n in params) if params else "void"
    return f"uint32_t encode_{ins.mnemonic}({param_str})"


def _gen_encode_instr(ins) -> list[str]:
    params  = _encode_params(ins)
    proto   = _encode_proto(ins)
    lines   = [proto + " {"]

    if ins.fmt == FMT_R:
        # Нужно разложить операнды по rs/rt/rd/shamt
        rs_val    = "0"
        rt_val    = "0"
        rd_val    = "0"
        shamt_val = "0"

        for op in ins.operands:
            n = op.name
            if isinstance(op, RegisterOperand):
                if n == "rs":   rs_val    = "rs"
                elif n == "rt": rt_val    = "rt"
                elif n == "rd": rd_val    = "rd"
            elif isinstance(op, ImmediateOperand) and n == "shamt":
                shamt_val = "shamt"

        lines.append(
            f"    return encode_r({rs_val}, {rt_val}, {rd_val}, "
            f"{shamt_val}, 0x{ins.funct:02X}u);"
        )

    elif ins.fmt == FMT_I:
        rs_val = "0"
        rt_val = "0"
        imm_val = "0"

        for op in ins.operands:
            n = op.name
            if isinstance(op, RegisterOperand):
                if n in ("rs", "base"): rs_val = n
                elif n == "rt":         rt_val = "rt"
            elif isinstance(op, ImmediateOperand):
                if n in ("imm", "offset", "csr"):
                    imm_val = n
            elif isinstance(op, LabelOperand):
                imm_val = op.name

        # subtype инструкций типа mfc0/mtc0 кодируется в rs
        if ins.subtype is not None:
            rs_val = f"0x{ins.subtype:02X}u"

        lines.append(
            f"    return encode_i(0x{ins.opcode:02X}u, "
            f"{rs_val}, {rt_val}, (uint16_t){imm_val});"
        )

    elif ins.fmt == FMT_J:
        target_val = "0"
        for op in ins.operands:
            if isinstance(op, LabelOperand):
                target_val = op.name

        lines.append(
            f"    return encode_j_raw(0x{ins.opcode:02X}u, {target_val});"
        )

    lines.append("}")
    lines.append("")
    return lines

@generator("c_codec")
def run(out_dir: str) -> None:
    for filename, content in [
        ("codec.h", gen_header()),
        ("codec.c", gen_source()),
    ]:
        out_path = os.path.join(out_dir, filename)
        with open(out_path, "w") as f:
            f.write(content)
        print(f"[c_codec] written to {out_path}")
 

