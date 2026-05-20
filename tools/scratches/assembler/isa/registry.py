from isa.instructions import *


INSTRUCTIONS = {
    "add": ADD,
    "sub": SUB,

    "and": AND,
    "or": OR,
    "xor": XOR,
    "nor": NOR,

    "slt": SLT,
    "sltu": SLTU,

    "sll": SLL,
    "srl": SRL,
    "sra": SRA,

    "sllv": SLLV,
    "srlv": SRLV,
    "srav": SRAV,

    "mul": MUL,
    "mulh": MULH,

    "div": DIV,
    "divu": DIVU,

    "rem": REM,
    "remu": REMU,

    "jr": JR,
    "jalr": JALR,

    "syscall": SYSCALL,
    "eret": ERET,

    "tlbwr": TLBWR,
    "tlbr": TLBR,
    "tlbp": TLBP,
    "tlbinv": TLBINV,

    "fence": FENCE,
    "wfi": WFI,
    "nop": NOP,
    "halt": HALT,

    "addi": ADDI,

    "andi": ANDI,
    "ori": ORI,
    "xori": XORI,

    "slti": SLTI,
    "sltiu": SLTIU,

    "lui": LUI,

    "beq": BEQ,
    "bne": BNE,

    "blt": BLT,
    "bge": BGE,

    "bltu": BLTU,
    "bgeu": BGEU,

    "lb": LB,
    "lh": LH,
    "lw": LW,

    "lbu": LBU,
    "lhu": LHU,

    "sb": SB,
    "sh": SH,
    "sw": SW,

    "ll": LL,
    "sc": SC,

    "mfc0": MFC0,
    "mtc0": MTC0,

    "j": J,
    "jal": JAL,
}


REGISTERS = {
    "r0": 0,
    "r1": 1,
    "r2": 2,
    "r3": 3,
    "r4": 4,
    "r5": 5,
    "r6": 6,
    "r7": 7,
    "r8": 8,
    "r9": 9,
    "r10": 10,
    "r11": 11,
    "r12": 12,
    "r13": 13,
    "r14": 14,
    "r15": 15,
    "r16": 16,
    "r17": 17,
    "r18": 18,
    "r19": 19,
    "r20": 20,
    "r21": 21,
    "r22": 22,
    "r23": 23,
    "r24": 24,
    "r25": 25,
    "r26": 26,
    "r27": 27,
    "r28": 28,
    "r29": 29,
    "r30": 30,
    "r31": 31,

    "zero": 0,
    "ra": 31,
}
