from isa.core import *

def r_type(
    mnemonic,
    funct,
    operands,
    subtype="R_STD"
):
    return instr(
        mnemonic=mnemonic,

        fmt=FMT_R,

        opcode=0x00,
        funct=funct,

        operands=operands,

        encoding=R_FORMAT,

        subtype=subtype
    )


def i_type(
    mnemonic,
    opcode,
    operands,
    subtype="I_STD"
):
    return instr(
        mnemonic=mnemonic,

        fmt=FMT_I,

        opcode=opcode,
        funct=0,

        operands=operands,

        encoding=I_FORMAT,

        subtype=subtype
    )


def j_type(
    mnemonic,
    opcode,
    operands,
    subtype="J_STD"
):
    return instr(
        mnemonic=mnemonic,

        fmt=FMT_J,

        opcode=opcode,
        funct=0,

        operands=operands,

        encoding=J_FORMAT,

        subtype=subtype
    )
