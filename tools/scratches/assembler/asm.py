from parser.parser import *
from isa.registry import *
from isa.encoder import *
from backend.binary import *


import re

from isa.registry import *


def parse_operand(text):
    #
    # Register
    #

    if text in REGISTERS:
        return REGISTERS[text]

    #
    # Integer immediate
    #

    if isinstance(text, int):
        return text

    #
    # Decimal immediate
    #

    if re.match(r"^-?[0-9]+$", text):
        return int(text)

    #
    # Hex immediate
    #

    if re.match(r"^0x[0-9a-fA-F]+$", text):
        return int(text, 16)

    #
    # offset(base)
    #
    # Example:
    #   4(r3)
    #   -8(sp)
    #

    m = re.match(r"^(-?[0-9]+)\(([^)]+)\)$", text)

    if m:
        offset = int(m.group(1))
        base   = m.group(2)

        if base not in REGISTERS:
            raise Exception(
                "unknown register: %s" % base
            )

        return (
            offset,
            REGISTERS[base]
        )

    raise Exception(
        "unknown operand: %s" % text
    )


def assemble_line(node):
    desc = INSTRUCTIONS[node.mnemonic]

    operands = []

    for operand in node.operands:
        value = parse_operand(operand)

        #
        # Memory operand:
        #   offset(base)
        #

        if isinstance(value, tuple):
            offset, base = value

            operands.append(offset)
            operands.append(base)

        else:
            operands.append(value)

    return encode_instruction(desc, operands)

def main():
    nodes = []

    with open("test.asm") as f:
        for line in f:
            line = line.strip()

            if not line:
                continue

            node = parse_line(line)

            nodes.append(node)

    words = []

    for node in nodes:
        if hasattr(node, "mnemonic"):
            words.append(
                assemble_line(node)
            )

    write_binary(words, "a.bin")

    print("assembled %d instructions" % len(words))


if __name__ == "__main__":
    main()
