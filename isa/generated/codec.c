/* Автогенерировано из isa/. Не редактировать вручную. */
#include "codec.h"
#include <stdint.h>

void decode_r(uint32_t instr, decoded_instruction_t *d) {
    d->raw = instr;
    d->opcode   = FIELD_OPCODE_GET(instr);
    d->rs       = FIELD_RS_GET(instr);
    d->rt       = FIELD_RT_GET(instr);
    d->rd       = FIELD_RD_GET(instr);
    d->shamt    = FIELD_SHAMT_GET(instr);
    d->funct    = FIELD_FUNCT_GET(instr);
}

void decode_i(uint32_t instr, decoded_instruction_t *d) {
    d->raw = instr;
    d->opcode   = FIELD_OPCODE_GET(instr);
    d->rs       = FIELD_RS_GET(instr);
    d->rt       = FIELD_RT_GET(instr);
    d->imm      = FIELD_IMM_GET(instr);
}

void decode_j(uint32_t instr, decoded_instruction_t *d) {
    d->raw = instr;
    d->opcode   = FIELD_OPCODE_GET(instr);
    d->target   = FIELD_TARGET_GET(instr);
}

void decode(uint32_t instr, decoded_instruction_t *d) {
    uint8_t opcode = FIELD_OPCODE_GET(instr);

    switch (opcode) {
    case 0x00: decode_r(instr, d); break;
    case 0x02: decode_j(instr, d); break;
    case 0x03: decode_j(instr, d); break;
    case 0x04: decode_i(instr, d); break;
    case 0x05: decode_i(instr, d); break;
    case 0x06: decode_i(instr, d); break;
    case 0x07: decode_i(instr, d); break;
    case 0x08: decode_i(instr, d); break;
    case 0x0A: decode_i(instr, d); break;
    case 0x0B: decode_i(instr, d); break;
    case 0x0C: decode_i(instr, d); break;
    case 0x0D: decode_i(instr, d); break;
    case 0x0E: decode_i(instr, d); break;
    case 0x0F: decode_i(instr, d); break;
    case 0x10: decode_i(instr, d); break;
    case 0x16: decode_i(instr, d); break;
    case 0x17: decode_i(instr, d); break;
    case 0x20: decode_i(instr, d); break;
    case 0x21: decode_i(instr, d); break;
    case 0x22: decode_i(instr, d); break;
    case 0x24: decode_i(instr, d); break;
    case 0x25: decode_i(instr, d); break;
    case 0x28: decode_i(instr, d); break;
    case 0x29: decode_i(instr, d); break;
    case 0x2A: decode_i(instr, d); break;
    case 0x30: decode_i(instr, d); break;
    case 0x31: decode_i(instr, d); break;
    default:
        /* Неизвестный опкод — заполняем raw, opcode; остальное 0 */
        d->raw    = instr;
        d->opcode = opcode;
        d->rs = d->rt = d->rd = d->shamt = d->funct = 0;
        d->imm    = 0;
        d->target = 0;
        break;
    }
}

uint32_t encode_r(uint8_t rs, uint8_t rt, uint8_t rd,
                  uint8_t shamt, uint8_t funct) {
    return FIELD_OPCODE_PUT(0x00u)
         | FIELD_RS_PUT(rs)
         | FIELD_RT_PUT(rt)
         | FIELD_RD_PUT(rd)
         | FIELD_SHAMT_PUT(shamt)
         | FIELD_FUNCT_PUT(funct);
}

uint32_t encode_i(uint8_t opcode, uint8_t rs, uint8_t rt,
                  uint16_t imm) {
    return FIELD_OPCODE_PUT(opcode)
         | FIELD_RS_PUT(rs)
         | FIELD_RT_PUT(rt)
         | FIELD_IMM_PUT(imm);
}

uint32_t encode_j_raw(uint8_t opcode, uint32_t target) {
    return FIELD_OPCODE_PUT(opcode)
         | FIELD_TARGET_PUT(target);
}

uint32_t encode_add(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x20u);
}

uint32_t encode_sub(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x22u);
}

uint32_t encode_and(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x24u);
}

uint32_t encode_or(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x25u);
}

uint32_t encode_xor(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x26u);
}

uint32_t encode_nor(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x27u);
}

uint32_t encode_slt(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x2Au);
}

uint32_t encode_sltu(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x2Bu);
}

uint32_t encode_sll(uint8_t rd, uint8_t rt, uint8_t shamt) {
    return encode_r(0, rt, rd, shamt, 0x00u);
}

uint32_t encode_srl(uint8_t rd, uint8_t rt, uint8_t shamt) {
    return encode_r(0, rt, rd, shamt, 0x02u);
}

uint32_t encode_sra(uint8_t rd, uint8_t rt, uint8_t shamt) {
    return encode_r(0, rt, rd, shamt, 0x03u);
}

uint32_t encode_sllv(uint8_t rd, uint8_t rt, uint8_t rs) {
    return encode_r(rs, rt, rd, 0, 0x04u);
}

uint32_t encode_srlv(uint8_t rd, uint8_t rt, uint8_t rs) {
    return encode_r(rs, rt, rd, 0, 0x06u);
}

uint32_t encode_srav(uint8_t rd, uint8_t rt, uint8_t rs) {
    return encode_r(rs, rt, rd, 0, 0x07u);
}

uint32_t encode_mul(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x18u);
}

uint32_t encode_mulh(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x19u);
}

uint32_t encode_div(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x1Au);
}

uint32_t encode_divu(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x1Bu);
}

uint32_t encode_rem(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x1Cu);
}

uint32_t encode_remu(uint8_t rd, uint8_t rs, uint8_t rt) {
    return encode_r(rs, rt, rd, 0, 0x1Du);
}

uint32_t encode_addi(uint8_t rt, uint8_t rs, int16_t imm) {
    return encode_i(0x08u, rs, rt, (uint16_t)imm);
}

uint32_t encode_andi(uint8_t rt, uint8_t rs, uint16_t imm) {
    return encode_i(0x0Cu, rs, rt, (uint16_t)imm);
}

uint32_t encode_ori(uint8_t rt, uint8_t rs, uint16_t imm) {
    return encode_i(0x0Du, rs, rt, (uint16_t)imm);
}

uint32_t encode_xori(uint8_t rt, uint8_t rs, uint16_t imm) {
    return encode_i(0x0Eu, rs, rt, (uint16_t)imm);
}

uint32_t encode_slti(uint8_t rt, uint8_t rs, int16_t imm) {
    return encode_i(0x0Au, rs, rt, (uint16_t)imm);
}

uint32_t encode_sltiu(uint8_t rt, uint8_t rs, uint16_t imm) {
    return encode_i(0x0Bu, rs, rt, (uint16_t)imm);
}

uint32_t encode_lui(uint8_t rt, uint16_t imm) {
    return encode_i(0x0Fu, 0, rt, (uint16_t)imm);
}

uint32_t encode_lb(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x20u, rs, rt, (uint16_t)offset);
}

uint32_t encode_lh(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x21u, rs, rt, (uint16_t)offset);
}

uint32_t encode_lw(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x22u, rs, rt, (uint16_t)offset);
}

uint32_t encode_lbu(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x24u, rs, rt, (uint16_t)offset);
}

uint32_t encode_lhu(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x25u, rs, rt, (uint16_t)offset);
}

uint32_t encode_sb(uint8_t rs, int16_t offset, uint8_t rt) {
    return encode_i(0x28u, rs, rt, (uint16_t)offset);
}

uint32_t encode_sh(uint8_t rs, int16_t offset, uint8_t rt) {
    return encode_i(0x29u, rs, rt, (uint16_t)offset);
}

uint32_t encode_sw(uint8_t rs, int16_t offset, uint8_t rt) {
    return encode_i(0x2Au, rs, rt, (uint16_t)offset);
}

uint32_t encode_ll(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x30u, rs, rt, (uint16_t)offset);
}

uint32_t encode_sc(uint8_t rt, int16_t offset, uint8_t rs) {
    return encode_i(0x31u, rs, rt, (uint16_t)offset);
}

uint32_t encode_beq(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x04u, rs, rt, (uint16_t)offset);
}

uint32_t encode_bne(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x05u, rs, rt, (uint16_t)offset);
}

uint32_t encode_blt(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x06u, rs, rt, (uint16_t)offset);
}

uint32_t encode_bge(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x07u, rs, rt, (uint16_t)offset);
}

uint32_t encode_bltu(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x16u, rs, rt, (uint16_t)offset);
}

uint32_t encode_bgeu(uint8_t rs, uint8_t rt, int16_t offset) {
    return encode_i(0x17u, rs, rt, (uint16_t)offset);
}

uint32_t encode_j(int16_t target) {
    return encode_j_raw(0x02u, target);
}

uint32_t encode_jal(int16_t target) {
    return encode_j_raw(0x03u, target);
}

uint32_t encode_jr(uint8_t rs) {
    return encode_r(rs, 0, 0, 0, 0x08u);
}

uint32_t encode_jalr(uint8_t rd, uint8_t rs) {
    return encode_r(rs, 0, rd, 0, 0x09u);
}

uint32_t encode_syscall(void) {
    return encode_r(0, 0, 0, 0, 0x0Cu);
}

uint32_t encode_eret(void) {
    return encode_r(0, 0, 0, 0, 0x0Du);
}

uint32_t encode_mfc0(uint8_t rt, uint8_t csr) {
    return encode_i(0x10u, 0x00u, rt, (uint16_t)csr);
}

uint32_t encode_mtc0(uint8_t rs, uint8_t csr) {
    return encode_i(0x10u, 0x04u, 0, (uint16_t)csr);
}

uint32_t encode_tlbwr(void) {
    return encode_r(0, 0, 0, 0, 0x38u);
}

uint32_t encode_tlbr(void) {
    return encode_r(0, 0, 0, 0, 0x39u);
}

uint32_t encode_tlbp(void) {
    return encode_r(0, 0, 0, 0, 0x3Au);
}

uint32_t encode_tlbinv(uint8_t rs) {
    return encode_r(rs, 0, 0, 0, 0x3Bu);
}

uint32_t encode_fence(void) {
    return encode_r(0, 0, 0, 0, 0x3Cu);
}

uint32_t encode_wfi(void) {
    return encode_r(0, 0, 0, 0, 0x3Du);
}

uint32_t encode_nop(void) {
    return encode_r(0, 0, 0, 0, 0x3Eu);
}

uint32_t encode_halt(void) {
    return encode_r(0, 0, 0, 0, 0x3Fu);
}
