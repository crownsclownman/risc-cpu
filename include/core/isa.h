#ifndef ISA_H
#define ISA_H

typedef enum {
    FMT_R,
    FMT_I,
    FMT_J,
    FMT_PSEUDO,
    FMT_UNKNOWN
} instruction_format_t;

#define NUM_REGS 32
#define WORD_SIZE 4
#define IMM16_MIN (-32768)
#define IMM16_MAX 65535

enum OP {
    OP_SPECIAL = 0x00,

    OP_J       = 0x02,
    OP_JAL     = 0x03,

    OP_BEQ     = 0x04,
    OP_BNE     = 0x05,
    OP_BLT     = 0x06,
    OP_BGE     = 0x07,

    OP_ADDI    = 0x08,
    OP_SLTI    = 0x0A,
    OP_SLTIU   = 0x0B,

    OP_ANDI    = 0x0C,
    OP_ORI     = 0x0D,
    OP_XORI    = 0x0E,

    OP_SYSTEM  = 0x10,

    OP_BLTU    = 0x16,
    OP_BGEU    = 0x17,

    OP_LB      = 0x20,
    OP_LH      = 0x21,
    OP_LW      = 0x22,

    OP_LBU     = 0x24,
    OP_LHU     = 0x25,

    OP_SB      = 0x28,
    OP_SH      = 0x29,
    OP_SW      = 0x2A,

    OP_LL      = 0x30,
    OP_SC      = 0x31,

    /*OP_LUI     = 0x0F*/
};

enum FUNCT {
    FUNCT_SLL     = 0x00,
    FUNCT_SRL     = 0x02,
    FUNCT_SRA     = 0x03,

    FUNCT_SLLV    = 0x04,
    FUNCT_SRLV    = 0x06,
    FUNCT_SRAV    = 0x07,

    FUNCT_JR      = 0x08,
    FUNCT_JALR    = 0x09,

    FUNCT_SYSCALL = 0x0C,
    FUNCT_ERET    = 0x0D,

    FUNCT_MUL     = 0x18,
    FUNCT_MULH    = 0x19,

    FUNCT_DIV     = 0x1A,
    FUNCT_DIVU    = 0x1B,

    FUNCT_REM     = 0x1C,
    FUNCT_REMU    = 0x1D,

    FUNCT_ADD     = 0x20,
    FUNCT_SUB     = 0x22,

    FUNCT_AND     = 0x24,
    FUNCT_OR      = 0x25,
    FUNCT_XOR     = 0x26,
    FUNCT_NOR     = 0x27,

    FUNCT_SLT     = 0x2A,
    FUNCT_SLTU    = 0x2B,

    FUNCT_TLBWR   = 0x38,
    FUNCT_TLBR    = 0x39,
    FUNCT_TLBP    = 0x3A,
    FUNCT_TLBINV  = 0x3B,

    FUNCT_FENCE   = 0x3C,
    FUNCT_WFI     = 0x3D,
    FUNCT_NOP     = 0x3E,
    FUNCT_HALT    = 0x3F
};

typedef enum {
    ISUB_R_STD,     /*  rd, rs1, rs2    */
    ISUB_R_SHIFT,   /*  rd, rs1, shamt  */
    ISUB_R_JR,      /*  rs1             */
    ISUB_R_JALR,    /*  rd, rs1         */
    ISUB_R_NOP,     /*  -               */

    ISUB_I_STD,     /*  rd, rs1, imm    */
    ISUB_I_BRANCH,  /*  rs1, rs2, label */
    ISUB_I_LOAD,    /*  rd, rs1, imm    */
    ISUB_I_STORE,   /*  rs, rs1, imm    */
    ISUB_I_LUI,     /*  rd, imm         */

    ISUB_J_STD,     /*  label           */

    ISUB_PSEUDO     /*  -               */
} instruction_subtype_t;

typedef struct {
    const char *mnemonic;
    instruction_format_t fmt;
    instruction_subtype_t sub;
    uint8_t opcode; /* funct for R-type */
} isa_entry_t;

#endif
