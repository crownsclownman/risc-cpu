from parser.ast import *
from parser.lexer import *

def parse_line(line):
    tokens = tokenize(line)

    #
    # Label
    #

    if (
        len(tokens) >= 3 and
        tokens[0].typ == TOKEN_IDENT and
        tokens[1].typ == TOKEN_COLON
    ):
        return LabelNode(tokens[0].value)

    #
    # Instruction
    #

    mnemonic = tokens[0].value

    operands = []

    for token in tokens[1:]:
        if token.typ in (TOKEN_COMMA, TOKEN_EOL):
            continue

        operands.append(token.value)

    return InstructionNode(
        mnemonic,
        operands
    )
