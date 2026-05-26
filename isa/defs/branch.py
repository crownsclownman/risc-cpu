from isa.core import *

instr(
    mnemonic = "beq",

    fmt = FMT_I,

    opcode = 0x04,

    operands = [
        reg("rs"),
        reg("rt"),
        label("target")
    ],

    encoding = I_FORMAT
)
