/* Автогенерировано из isa/. Не редактировать вручную. */
#pragma once
#include <stdint.h>


/* R-type */
#define FIELD_OPCODE_SHIFT  26u
#define FIELD_OPCODE_MASK   0x3Fu
#define FIELD_OPCODE_GET(w) (((w) >> 26u) & 0x3Fu)
#define FIELD_OPCODE_PUT(v) (((uint32_t)(v) & 0x3Fu) << 26u)

#define FIELD_RS_SHIFT  21u
#define FIELD_RS_MASK   0x1Fu
#define FIELD_RS_GET(w) (((w) >> 21u) & 0x1Fu)
#define FIELD_RS_PUT(v) (((uint32_t)(v) & 0x1Fu) << 21u)

#define FIELD_RT_SHIFT  16u
#define FIELD_RT_MASK   0x1Fu
#define FIELD_RT_GET(w) (((w) >> 16u) & 0x1Fu)
#define FIELD_RT_PUT(v) (((uint32_t)(v) & 0x1Fu) << 16u)

#define FIELD_RD_SHIFT  11u
#define FIELD_RD_MASK   0x1Fu
#define FIELD_RD_GET(w) (((w) >> 11u) & 0x1Fu)
#define FIELD_RD_PUT(v) (((uint32_t)(v) & 0x1Fu) << 11u)

#define FIELD_SHAMT_SHIFT  6u
#define FIELD_SHAMT_MASK   0x1Fu
#define FIELD_SHAMT_GET(w) (((w) >> 6u) & 0x1Fu)
#define FIELD_SHAMT_PUT(v) (((uint32_t)(v) & 0x1Fu) << 6u)

#define FIELD_FUNCT_SHIFT  0u
#define FIELD_FUNCT_MASK   0x3Fu
#define FIELD_FUNCT_GET(w) (((w) >> 0u) & 0x3Fu)
#define FIELD_FUNCT_PUT(v) (((uint32_t)(v) & 0x3Fu) << 0u)


/* I-type */
#define FIELD_IMM_SHIFT  0u
#define FIELD_IMM_MASK   0xFFFFu
#define FIELD_IMM_GET(w) (((w) >> 0u) & 0xFFFFu)
#define FIELD_IMM_PUT(v) (((uint32_t)(v) & 0xFFFFu) << 0u)


/* J-type */
#define FIELD_TARGET_SHIFT  0u
#define FIELD_TARGET_MASK   0x3FFFFFFu
#define FIELD_TARGET_GET(w) (((w) >> 0u) & 0x3FFFFFFu)
#define FIELD_TARGET_PUT(v) (((uint32_t)(v) & 0x3FFFFFFu) << 0u)


typedef struct {
    uint32_t raw;

    /* Общие поля (присутствуют во всех форматах) */
    uint8_t  opcode;

    /* R-type */
    uint8_t  rs;
    uint8_t  rt;
    uint8_t  rd;
    uint8_t  shamt;
    uint8_t  funct;

    /* I-type */
    uint16_t imm;

    /* J-type */
    uint32_t target;
} decoded_instruction_t;

void decode  (uint32_t instr, decoded_instruction_t *d);
void decode_r(uint32_t instr, decoded_instruction_t *d);
void decode_i(uint32_t instr, decoded_instruction_t *d);
void decode_j(uint32_t instr, decoded_instruction_t *d);

uint32_t encode_r(uint8_t rs, uint8_t rt, uint8_t rd,
                  uint8_t shamt, uint8_t funct);
uint32_t encode_i(uint8_t opcode, uint8_t rs, uint8_t rt,
                  uint16_t imm);
uint32_t encode_j_raw(uint8_t opcode, uint32_t target);

uint32_t encode_add(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_sub(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_and(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_or(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_xor(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_nor(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_slt(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_sltu(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_sll(uint8_t rd, uint8_t rt, uint8_t shamt);
uint32_t encode_srl(uint8_t rd, uint8_t rt, uint8_t shamt);
uint32_t encode_sra(uint8_t rd, uint8_t rt, uint8_t shamt);
uint32_t encode_sllv(uint8_t rd, uint8_t rt, uint8_t rs);
uint32_t encode_srlv(uint8_t rd, uint8_t rt, uint8_t rs);
uint32_t encode_srav(uint8_t rd, uint8_t rt, uint8_t rs);
uint32_t encode_mul(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_mulh(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_div(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_divu(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_rem(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_remu(uint8_t rd, uint8_t rs, uint8_t rt);
uint32_t encode_addi(uint8_t rt, uint8_t rs, int16_t imm);
uint32_t encode_andi(uint8_t rt, uint8_t rs, uint16_t imm);
uint32_t encode_ori(uint8_t rt, uint8_t rs, uint16_t imm);
uint32_t encode_xori(uint8_t rt, uint8_t rs, uint16_t imm);
uint32_t encode_slti(uint8_t rt, uint8_t rs, int16_t imm);
uint32_t encode_sltiu(uint8_t rt, uint8_t rs, uint16_t imm);
uint32_t encode_lui(uint8_t rt, uint16_t imm);
uint32_t encode_lb(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_lh(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_lw(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_lbu(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_lhu(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_sb(uint8_t rs, int16_t offset, uint8_t rt);
uint32_t encode_sh(uint8_t rs, int16_t offset, uint8_t rt);
uint32_t encode_sw(uint8_t rs, int16_t offset, uint8_t rt);
uint32_t encode_ll(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_sc(uint8_t rt, int16_t offset, uint8_t rs);
uint32_t encode_beq(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_bne(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_blt(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_bge(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_bltu(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_bgeu(uint8_t rs, uint8_t rt, int16_t offset);
uint32_t encode_j(int16_t target);
uint32_t encode_jal(int16_t target);
uint32_t encode_jr(uint8_t rs);
uint32_t encode_jalr(uint8_t rd, uint8_t rs);
uint32_t encode_syscall(void);
uint32_t encode_eret(void);
uint32_t encode_mfc0(uint8_t rt, uint8_t csr);
uint32_t encode_mtc0(uint8_t rs, uint8_t csr);
uint32_t encode_tlbwr(void);
uint32_t encode_tlbr(void);
uint32_t encode_tlbp(void);
uint32_t encode_tlbinv(uint8_t rs);
uint32_t encode_fence(void);
uint32_t encode_wfi(void);
uint32_t encode_nop(void);
uint32_t encode_halt(void);
