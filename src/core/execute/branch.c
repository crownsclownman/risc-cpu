#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"

void execute_branch(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rs;
    uint32_t rt;

    int32_t srs;
    int32_t srt;
    int32_t simm;

    uint32_t next_pc;

    rs = arch->registers[d->rs];
    rt = arch->registers[d->rt];

    srs = (int32_t)rs;
    srt = (int32_t)rt;

    simm = sign_ext16(d->imm);

    next_pc = arch->pc + 4;

    switch (d->opcode) {

    case OP_BEQ:
        if (rs == rt)
            next_pc = branch_target(arch, simm);
        break;

    case OP_BNE:
        if (rs != rt)
            next_pc = branch_target(arch, simm);
        break;

    case OP_BLT:
        if (srs < srt)
            next_pc = branch_target(arch, simm);
        break;

    case OP_BGE:
        if (srs >= srt)
            next_pc = branch_target(arch, simm);
        break;

    case OP_BLTU:
        if (rs < rt)
            next_pc = branch_target(arch, simm);
        break;

    case OP_BGEU:
        if (rs >= rt)
            next_pc = branch_target(arch, simm);
        break;

    default:
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        return;
    }

    arch->pc = next_pc;
}

void execute_jump(cpu_t *arch,
                  decoded_instruction_t *d)
{
    uint32_t rs;
    uint32_t next_pc;

    rs = arch->registers[d->rs];

    next_pc = arch->pc + 4;

    switch (d->opcode) {

    case OP_J:
        next_pc = jump_target(arch, d->target);
        break;

    case OP_JAL:
        reg_write(arch, 31, arch->pc + 4);
        next_pc = jump_target(arch, d->target);
        break;

    case OP_SPECIAL:

        switch (d->funct) {

        case FUNCT_JR:
            next_pc = rs;
            break;

        case FUNCT_JALR:
            reg_write(arch, d->rd, arch->pc + 4);
            next_pc = rs;
            break;

        default:
            cpu_raise_exception(arch, CAUSE_ILL_INSTR);
            return;
        }

        break;

    default:
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        return;
    }

    arch->pc = next_pc;
}
