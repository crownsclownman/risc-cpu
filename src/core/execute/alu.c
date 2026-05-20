#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"

void execute_special_alu(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rs;
    uint32_t rt;

    int32_t srs;
    int32_t srt;

    rs = arch->registers[d->rs];
    rt = arch->registers[d->rt];

    srs = (int32_t)rs;
    srt = (int32_t)rt;

    switch (d->funct) {

    case FUNCT_SLL:
        reg_write(arch, d->rd, rt << d->shamt);
        break;

    case FUNCT_SRL:
        reg_write(arch, d->rd, rt >> d->shamt);
        break;

    case FUNCT_ADD:
        reg_write(arch, d->rd, rs + rt);
        break;

    case FUNCT_SUB:
        reg_write(arch, d->rd, rs - rt);
        break;

    default:
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        break;
    }
}

void execute_immediate_alu(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rs;
    int32_t  srs;
    int32_t  simm;

    rs   = arch->registers[d->rs];
    srs  = (int32_t)rs;
    simm = sign_ext16(d->imm);

    switch (d->opcode) {

    case OP_ADDI:
        reg_write(arch, d->rt, (uint32_t)(srs + simm));
        break;

    case OP_SLTI:
        reg_write(arch, d->rt, srs < simm ? 1u : 0u);
        break;

    case OP_SLTIU:
        reg_write(arch, d->rt, rs < (uint32_t)simm ? 1u : 0u);
        break;

    case OP_ANDI:
        reg_write(arch, d->rt, rs & (uint32_t)d->imm);
        break;

    case OP_ORI:
        reg_write(arch, d->rt, rs | (uint32_t)d->imm);
        break;

    case OP_XORI:
        reg_write(arch, d->rt, rs ^ (uint32_t)d->imm);
        break;

    default:
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        break;
    }
}
