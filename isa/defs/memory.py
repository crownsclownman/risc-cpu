from ..core.instruction import instr
from ..core.operands   import reg, imm
from ..core.formats    import I_FORMAT
from ..core.enums      import FMT_I

instr(
    mnemonic = "lb",
    fmt      = FMT_I,
    opcode   = 0x20,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "lh",
    fmt      = FMT_I,
    opcode   = 0x21,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "lw",
    fmt      = FMT_I,
    opcode   = 0x22,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "lbu",
    fmt      = FMT_I,
    opcode   = 0x24,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "lhu",
    fmt      = FMT_I,
    opcode   = 0x25,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "sb",
    fmt      = FMT_I,
    opcode   = 0x28,
    operands = [reg("rs"), imm("offset", 16, signed=True), reg("rt")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "sh",
    fmt      = FMT_I,
    opcode   = 0x29,
    operands = [reg("rs"), imm("offset", 16, signed=True), reg("rt")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "sw",
    fmt      = FMT_I,
    opcode   = 0x2A,
    operands = [reg("rs"), imm("offset", 16, signed=True), reg("rt")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "ll",
    fmt      = FMT_I,
    opcode   = 0x30,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "sc",
    fmt      = FMT_I,
    opcode   = 0x31,
    operands = [reg("rt"), imm("offset", 16, signed=True), reg("rs")],
    encoding = I_FORMAT,
)
