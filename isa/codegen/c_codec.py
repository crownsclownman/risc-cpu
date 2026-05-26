from isa.core.formats import R_FORMAT, I_FORMAT, J_FORMAT
from isa.core.fields import Field
from isa.core.registry import INSTRUCTIONS
from isa.core.enums import FMT_R, FMT_I, FMT_J


FORMATS = {
    "R": R_FORMAT,
    "I": I_FORMAT,
    "J": J_FORMAT,
}

def validate_format(fmt_name, fields):
    used = [0] * 32

    for f in fields:
        for i in range(f.offset, f.offset + f.width):
            if used[i]:
                raise ValueError(
                    f"BIT OVERLAP in {fmt_name}: {f.source}"
                )
            used[i] = 1

def validate_global_fields():
    field_map = {}

    for fmt_name, fields in FORMATS.items():
        for f in fields:
            field_map.setdefault(f.source, set()).add(fmt_name)

    for name, fmts in field_map.items():
        if len(fmts) > 3:
            print(f"[warn] field {name} in many formats: {fmts}")

def validate():
    for name, fields in FORMATS.items():
        validate_format(name, fields)

    validate_global_fields()

def gen_macros():
    lines = [
        "/* автосгенерировано через c_codec.py */",
        "",
    ]

    for fmt_name, fields in FORMATS.items():
        for f in fields:
            macro = f"GET_{f.source.upper()}_{fmt_name}"
            lines.append(
                f"#define {macro}(x) (((x) >> {f.offset}) & 0x{(1<<f.width)-1:X})"
            )

    return "\n".join(lines)

def gen_decode():
    lines = [
        "void decode(uint32_t instr, decoded_instruction_t *d)",
        "{",
        "    d->raw = instr;",
        "",
        "    uint8_t opcode = (instr >> 26) & 0x3F;",
        "    d->opcode = opcode;",
        "",
    ]

    def emit(fields):
        for f in fields:
            if f.source == "opcode":
                continue
            mask = (1 << f.width) - 1
            lines.append(
                f"    d->{f.source} = (instr >> {f.offset}) & 0x{mask:X};"
            )

    emit(R_FORMAT)
    emit(I_FORMAT)
    emit(J_FORMAT)

    lines.append("}")
    return "\n".join(lines)
