from dataclasses import dataclass
from isa.fields import Field

@dataclass
class Format:
    name: str
    fields: list[Field]


#
# Common fields
#

OPCODE = Field("opcode", 26, 6)

RS     = Field("rs",     21, 5)
RT     = Field("rt",     16, 5)
RD     = Field("rd",     11, 5)

SHAMT  = Field("shamt",   6, 5)
FUNCT  = Field("funct",   0, 6)

IMM16  = Field("imm16",   0, 16)

ADDRESS = Field("address", 0, 26)


#
# Formats
#

R_FORMAT = Format(
    "R",
    [
        OPCODE,
        RS,
        RT,
        RD,
        SHAMT,
        FUNCT,
    ]
)

I_FORMAT = Format(
    "I",
    [
        OPCODE,
        RS,
        RT,
        IMM16,
    ]
)

J_FORMAT = Format(
    "J",
    [
        OPCODE,
        ADDRESS,
    ]
)
