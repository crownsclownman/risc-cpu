from ..core.instruction import instr
from ..core.operands   import reg, branch
from ..core.formats    import I_FORMAT
from ..core.enums      import FMT_I

instr(
    mnemonic = "beq",
    fmt      = FMT_I,
    opcode   = 0x04,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "bne",
    fmt      = FMT_I,
    opcode   = 0x05,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "blt",
    fmt      = FMT_I,
    opcode   = 0x06,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "bge",
    fmt      = FMT_I,
    opcode   = 0x07,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "bltu",
    fmt      = FMT_I,
    opcode   = 0x16,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "bgeu",
    fmt      = FMT_I,
    opcode   = 0x17,
    operands = [reg("rs"), reg("rt"), branch()],
    encoding = I_FORMAT,
)
