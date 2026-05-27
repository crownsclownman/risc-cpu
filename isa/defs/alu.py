from ..core.instruction import instr
from ..core.operands   import reg, imm, shamt
from ..core.formats    import R_FORMAT, I_FORMAT
from ..core.enums      import FMT_R, FMT_I

instr(
    mnemonic = "add",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x20,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "sub",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x22,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "and",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x24,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "or",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x25,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "xor",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x26,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "nor",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x27,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "slt",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x2A,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "sltu",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x2B,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "sll",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x00,
    operands = [reg("rd"), reg("rt"), shamt()],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "srl",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x02,
    operands = [reg("rd"), reg("rt"), shamt()],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "sra",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x03,
    operands = [reg("rd"), reg("rt"), shamt()],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "sllv",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x04,
    operands = [reg("rd"), reg("rt"), reg("rs")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "srlv",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x06,
    operands = [reg("rd"), reg("rt"), reg("rs")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "srav",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x07,
    operands = [reg("rd"), reg("rt"), reg("rs")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "mul",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x18,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "mulh",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x19,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "div",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x1A,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "divu",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x1B,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "rem",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x1C,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "remu",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x1D,
    operands = [reg("rd"), reg("rs"), reg("rt")],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "addi",
    fmt      = FMT_I,
    opcode   = 0x08,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=True)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "andi",
    fmt      = FMT_I,
    opcode   = 0x0C,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=False)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "ori",
    fmt      = FMT_I,
    opcode   = 0x0D,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=False)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "xori",
    fmt      = FMT_I,
    opcode   = 0x0E,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=False)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "slti",
    fmt      = FMT_I,
    opcode   = 0x0A,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=True)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "sltiu",
    fmt      = FMT_I,
    opcode   = 0x0B,
    operands = [reg("rt"), reg("rs"), imm("imm", 16, signed=False)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "lui",
    fmt      = FMT_I,
    opcode   = 0x0F,               # TODO: раскомментировать OP_LUI в C
    operands = [reg("rt"), imm("imm", 16, signed=False)],
    encoding = I_FORMAT,
)
