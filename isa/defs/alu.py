from isa.core import *

r_type(
    "add",
    funct=0x20,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "sub",
    funct=0x22,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "and",
    funct=0x24,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "or",
    funct=0x25,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "xor",
    funct=0x26,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "nor",
    funct=0x27,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

#
# Compare
#

r_type(
    "slt",
    funct=0x2A,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)

r_type(
    "sltu",
    funct=0x2B,

    operands=[
        reg("rd"),
        reg("rs"),
        reg("rt")
    ]
)
