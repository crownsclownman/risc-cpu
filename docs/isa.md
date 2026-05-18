This architecture implements following instructions:

Memory:     LW, LH, LB, LHU, LBU, SW, SH, SB
Arithmetic: ADD, ADDI, SUB, ADDU, SUBU
Mul-Div:    MUL, MULH, DIV, DIVU, REM, REMU
Logic:      AND, ANDI, OR, ORI, XOR, XORI, NOR, NOP
Shifts:     SLL, SRL, SRA, SLLI, SRLI, SRAI
Compare:    SLT, SLTI, SLTU, SLTIU
Jumps:      J, JAL, JR, JALR
Branches:   BEQ, BNE, BLT, BGE, BLTU, BGEU
Atomic:     LL, SC
System:     SYSCALL, ERET, MCF0, MTC0, TLBWR, TLBR, TLBINV, FENCE, WFI, HALT

And following instruction encodings:

R-type:
    oooooo sssss ttttt ddddd shamt ffffff
        *o      - opcode
        *s      - register source; first operand
        *t      - register source; second operand
        *d      - register destination
        *shamt  - shift magnitude
        *f      - funct

I-type:
    oooooo sssss ttttt iiiiiiiiiiiiiiii
        *o      - instruction opcode
        *s      - base register; first operand
        *t      - register destination; second operand; register source
        *i      - 16-bit immediate

J-type:
    oooooo tttttttttttttttttttttttttt
        *o      - instruction opcode
        *t      - target
