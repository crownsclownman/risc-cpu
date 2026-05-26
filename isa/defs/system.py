from ..core.instruction import instr
from ..core.operands   import reg, imm
from ..core.formats    import R_FORMAT, I_FORMAT
from ..core.enums      import FMT_R, FMT_I

#Trap / return
instr(
    mnemonic = "syscall",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x0C,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "eret",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x0D,
    operands = [],
    encoding = R_FORMAT,
)

# CSR: MFC0 / MTC0  (OP_SYSTEM = 0x10)
# Кодируются через I_FORMAT:
#   opcode=0x10  rs=sub  rt=gp-reg  rd(биты [15:11])=csr-номер
instr(
    mnemonic = "mfc0",
    fmt      = FMT_I,
    opcode   = 0x10,
    subtype  = 0x00,               # sub-op: rs поле = 0x00
    operands = [reg("rt"), imm("csr", 5)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "mtc0",
    fmt      = FMT_I,
    opcode   = 0x10,
    subtype  = 0x04,               # sub-op: rs поле = 0x04
    operands = [reg("rs"), imm("csr", 5)],
    encoding = I_FORMAT,
)

instr(
    mnemonic = "tlbwr",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x38,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "tlbr",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x39,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "tlbp",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3A,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "tlbinv",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3B,
    operands = [reg("rs")],        # rs = виртуальный адрес
    encoding = R_FORMAT,
)

instr(
    mnemonic = "fence",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3C,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "wfi",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3D,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "nop",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3E,
    operands = [],
    encoding = R_FORMAT,
)

instr(
    mnemonic = "halt",
    fmt      = FMT_R,
    opcode   = 0x00,
    funct    = 0x3F,
    operands = [],
    encoding = R_FORMAT,
)
