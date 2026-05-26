from isa.core.bits import get_bits, set_bits


def encode(fields, values: dict) -> int:
    instr = 0

    for f in fields:
        if f.source not in values:
            continue

        instr = set_bits(
            instr,
            values[f.source],
            f.offset,
            f.width
        )

    return instr


def decode(fields, instr: int) -> dict:
    out = {}

    for f in fields:
        out[f.source] = get_bits(instr, f.offset, f.width)

    return out
