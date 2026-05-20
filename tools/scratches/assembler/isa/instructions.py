from dataclasses import dataclass

from isa.formats import *
from isa.operands import *

@dataclass
class OperandMapping:
    operand_index: int
    field_name: str

@dataclass
class Instruction:
    mnemonic: str

    fmt: Format

    fixed_fields: dict

    operand_types: list

    operand_map: list

#
# Generic builders
#

def r_type(name, funct):
    return Instruction(
        mnemonic=name,

        fmt=R_FORMAT,

        fixed_fields={
            "opcode": 0x00,
            "shamt":  0x00,
            "funct":  funct,
        },

        operand_types=[
            REG,
            REG,
            REG,
        ],

        operand_map=[
            OperandMapping(0, "rd"),
            OperandMapping(1, "rs"),
            OperandMapping(2, "rt"),
        ]
    )


def shift_imm(name, funct):
    return Instruction(
        mnemonic=name,

        fmt=R_FORMAT,

        fixed_fields={
            "opcode": 0x00,
            "rs":     0x00,
            "funct":  funct,
        },

        operand_types=[
            REG,
            REG,
            SHAMT5,
        ],

        operand_map=[
            OperandMapping(0, "rd"),
            OperandMapping(1, "rt"),
            OperandMapping(2, "shamt"),
        ]
    )


def shift_reg(name, funct):
    return Instruction(
        mnemonic=name,

        fmt=R_FORMAT,

        fixed_fields={
            "opcode": 0x00,
            "shamt":  0x00,
            "funct":  funct,
        },

        operand_types=[
            REG,
            REG,
            REG,
        ],

        operand_map=[
            OperandMapping(0, "rd"),
            OperandMapping(1, "rt"),
            OperandMapping(2, "rs"),
        ]
    )


def i_type(name, opcode):
    return Instruction(
        mnemonic=name,

        fmt=I_FORMAT,

        fixed_fields={
            "opcode": opcode,
        },

        operand_types=[
            REG,
            REG,
            IMM16,
        ],

        operand_map=[
            OperandMapping(0, "rt"),
            OperandMapping(1, "rs"),
            OperandMapping(2, "imm16"),
        ]
    )


def branch_type(name, opcode):
    return Instruction(
        mnemonic=name,

        fmt=I_FORMAT,

        fixed_fields={
            "opcode": opcode,
        },

        operand_types=[
            REG,
            REG,
            LABEL,
        ],

        operand_map=[
            OperandMapping(0, "rs"),
            OperandMapping(1, "rt"),
            OperandMapping(2, "imm16"),
        ]
    )


def load_type(name, opcode):
    return Instruction(
        mnemonic=name,

        fmt=I_FORMAT,

        fixed_fields={
            "opcode": opcode,
        },

        operand_types=[
            REG,
            IMM16,
            REG,
        ],

        operand_map=[
            OperandMapping(0, "rt"),
            OperandMapping(1, "imm16"),
            OperandMapping(2, "rs"),
        ]
    )


def store_type(name, opcode):
    return Instruction(
        mnemonic=name,

        fmt=I_FORMAT,

        fixed_fields={
            "opcode": opcode,
        },

        operand_types=[
            REG,
            IMM16,
            REG,
        ],

        operand_map=[
            OperandMapping(0, "rt"),
            OperandMapping(1, "imm16"),
            OperandMapping(2, "rs"),
        ]
    )


def j_type(name, opcode):
    return Instruction(
        mnemonic=name,

        fmt=J_FORMAT,

        fixed_fields={
            "opcode": opcode,
        },

        operand_types=[
            LABEL,
        ],

        operand_map=[
            OperandMapping(0, "address"),
        ]
    )


def special_noarg(name, funct):
    return Instruction(
        mnemonic=name,

        fmt=R_FORMAT,

        fixed_fields={
            "opcode": 0x00,
            "rs":     0x00,
            "rt":     0x00,
            "rd":     0x00,
            "shamt":  0x00,
            "funct":  funct,
        },

        operand_types=[],
        operand_map=[]
    )


#
# ALU
#

ADD   = r_type("add",   0x20)
SUB   = r_type("sub",   0x22)

AND   = r_type("and",   0x24)
OR    = r_type("or",    0x25)
XOR   = r_type("xor",   0x26)
NOR   = r_type("nor",   0x27)

SLT   = r_type("slt",   0x2A)
SLTU  = r_type("sltu",  0x2B)

#
# Shifts
#

SLL   = shift_imm("sll",  0x00)
SRL   = shift_imm("srl",  0x02)
SRA   = shift_imm("sra",  0x03)

SLLV  = shift_reg("sllv", 0x04)
SRLV  = shift_reg("srlv", 0x06)
SRAV  = shift_reg("srav", 0x07)

#
# Mul/div
#

MUL   = r_type("mul",   0x18)
MULH  = r_type("mulh",  0x19)

DIV   = r_type("div",   0x1A)
DIVU  = r_type("divu",  0x1B)

REM   = r_type("rem",   0x1C)
REMU  = r_type("remu",  0x1D)

#
# Register jumps
#

JR = Instruction(
    mnemonic="jr",

    fmt=R_FORMAT,

    fixed_fields={
        "opcode": 0x00,
        "rt":     0x00,
        "rd":     0x00,
        "shamt":  0x00,
        "funct":  0x08,
    },

    operand_types=[
        REG,
    ],

    operand_map=[
        OperandMapping(0, "rs"),
    ]
)


JALR = Instruction(
    mnemonic="jalr",

    fmt=R_FORMAT,

    fixed_fields={
        "opcode": 0x00,
        "rt":     0x00,
        "shamt":  0x00,
        "funct":  0x09,
    },

    operand_types=[
        REG,
        REG,
    ],

    operand_map=[
        OperandMapping(0, "rd"),
        OperandMapping(1, "rs"),
    ]
)

#
# System
#

SYSCALL = special_noarg("syscall", 0x0C)
ERET    = special_noarg("eret",    0x0D)

TLBWR   = special_noarg("tlbwr",   0x38)
TLBR    = special_noarg("tlbr",    0x39)
TLBP    = special_noarg("tlbp",    0x3A)

FENCE   = special_noarg("fence",   0x3C)
WFI     = special_noarg("wfi",     0x3D)
NOP     = special_noarg("nop",     0x3E)
HALT    = special_noarg("halt",    0x3F)


TLBINV = Instruction(
    mnemonic="tlbinv",

    fmt=R_FORMAT,

    fixed_fields={
        "opcode": 0x00,
        "rt":     0x00,
        "rd":     0x00,
        "shamt":  0x00,
        "funct":  0x3B,
    },

    operand_types=[
        REG,
    ],

    operand_map=[
        OperandMapping(0, "rs"),
    ]
)

#
# Immediate ALU
#

ADDI  = i_type("addi",  0x08)

ANDI  = i_type("andi",  0x0C)
ORI   = i_type("ori",   0x0D)
XORI  = i_type("xori",  0x0E)

SLTI  = i_type("slti",  0x0A)
SLTIU = i_type("sltiu", 0x0B)

#
# LUI
#

LUI = Instruction(
    mnemonic="lui",

    fmt=I_FORMAT,

    fixed_fields={
        "opcode": 0x0F,
        "rs":     0x00,
    },

    operand_types=[
        REG,
        IMM16,
    ],

    operand_map=[
        OperandMapping(0, "rt"),
        OperandMapping(1, "imm16"),
    ]
)

#
# Branches
#

BEQ   = branch_type("beq",   0x04)
BNE   = branch_type("bne",   0x05)

BLT   = branch_type("blt",   0x06)
BGE   = branch_type("bge",   0x07)

BLTU  = branch_type("bltu",  0x16)
BGEU  = branch_type("bgeu",  0x17)

#
# Loads
#

LB    = load_type("lb",   0x20)
LH    = load_type("lh",   0x21)
LW    = load_type("lw",   0x22)

LBU   = load_type("lbu",  0x24)
LHU   = load_type("lhu",  0x25)

#
# Stores
#

SB    = store_type("sb",  0x28)
SH    = store_type("sh",  0x29)
SW    = store_type("sw",  0x2A)

#
# Atomic
#

LL = load_type("ll", 0x30)
SC = load_type("sc", 0x31)

#
# Coprocessor
#

MFC0 = Instruction(
    mnemonic="mfc0",

    fmt=I_FORMAT,

    fixed_fields={
        "opcode": 0x10,
        "rs":     0x00,
    },

    operand_types=[
        REG,
        IMM16,
    ],

    operand_map=[
        OperandMapping(0, "rt"),
        OperandMapping(1, "rd"),
    ]
)


MTC0 = Instruction(
    mnemonic="mtc0",

    fmt=I_FORMAT,

    fixed_fields={
        "opcode": 0x10,
        "rs":     0x04,
    },

    operand_types=[
        REG,
        IMM16,
    ],

    operand_map=[
        OperandMapping(0, "rt"),
        OperandMapping(1, "rd"),
    ]
)

#
# Jumps
#

J   = j_type("j",   0x02)
JAL = j_type("jal", 0x03)
