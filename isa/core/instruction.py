from .registry import INSTRUCTIONS

class Instruction:
    def __init__(
        self,
        mnemonic,
        fmt,

        opcode,
        funct,

        operands,
        encoding,

        subtype=None
    ):
        self.mnemonic = mnemonic

        self.fmt = fmt
        self.subtype = subtype

        self.opcode = opcode
        self.funct = funct

        self.operands = operands
        self.encoding = encoding

    def __repr__(self):
        return (
            f"Instruction("
            f"{self.mnemonic})"
        )

def instr(
    mnemonic,

    fmt,

    opcode,
    funct=0,

    operands=None,
    encoding=None,

    subtype=None
):
    if operands is None:
        operands = []

    if encoding is None:
        encoding = []

    ins = Instruction(
        mnemonic=mnemonic,

        fmt=fmt,
        subtype=subtype,

        opcode=opcode,
        funct=funct,

        operands=operands,
        encoding=encoding
    )

    INSTRUCTIONS.append(ins)

    return ins
