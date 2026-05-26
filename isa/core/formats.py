from .fields import field
from .codec import encode, decode

R_FORMAT = [
    field("opcode", 26, 6),
    field("rs",     21, 5),
    field("rt",     16, 5),
    field("rd",     11, 5),
    field("shamt",   6, 5),
    field("funct",   0, 6),
]

I_FORMAT = [
    field("opcode", 26, 6),
    field("rs",     21, 5),
    field("rt",     16, 5),
    field("imm",     0, 16),
]

J_FORMAT = [
    field("opcode", 26, 6),
    field("target",  0, 26),
]

def encode_r(values):
    return encode(R_FORMAT, values)


def decode_r(instr):
    return decode(R_FORMAT, instr)


def encode_i(values):
    return encode(I_FORMAT, values)


def decode_i(instr):
    return decode(I_FORMAT, instr)


def encode_j(values):
    return encode(J_FORMAT, values)


def decode_j(instr):
    return decode(J_FORMAT, instr)
