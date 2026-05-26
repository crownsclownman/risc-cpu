from .fields import field

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
