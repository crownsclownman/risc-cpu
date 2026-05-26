from ..core.instruction import instr
from ..core.operands   import reg, label
from ..core.formats    import J_FORMAT, R_FORMAT
from ..core.enums      import FMT_J, FMT_R

instr(
    mnemonic = "j",
    fmt      = FMT_J,
    opcode   = 0x02,
    operands = [label("target")],
    encoding = J_FORMAT,
)

instr(
    mnemonic = "jal",
    fmt      = FMT_J,
    opcode   = 0x03,
    operands = [label("target")],
    encoding = J_FORMAT,
)

instr(
    mnemonic = "jr",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x08,
    operands = [reg("rs")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "jalr",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x09,
    operands = [reg("rd"), reg("rs")],
    encoding = R_FORMAT,
)
