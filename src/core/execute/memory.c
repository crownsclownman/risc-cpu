#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"
#include "core/bus.h"

void execute_loadstore(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rs;
    uint32_t rt;

    int32_t srs;
    int32_t simm;

    uint32_t ea;

    rs = arch->registers[d->rs];
    rt = arch->registers[d->rt];

    srs = (int32_t)rs;

    simm = sign_ext16(d->imm);

    ea = (uint32_t)(srs + simm);

    switch (d->opcode) {

    case OP_LB:
        reg_write(arch, d->rt, (uint32_t)(int32_t)(int8_t)cpu_read8(arch, ea));
        break;

    case OP_LH:
        reg_write(arch, d->rt, (uint32_t)(int32_t)(int16_t)cpu_read16(arch, ea));
        break;

    case OP_LW:
        reg_write(arch, d->rt, cpu_read32(arch, ea));
        break;

    case OP_LBU:
        reg_write(arch, d->rt, (uint32_t)cpu_read8(arch, ea));
        break;

    case OP_LHU:
        reg_write(arch, d->rt, (uint32_t)cpu_read16(arch, ea));
        break;

    case OP_SB:
        cpu_write8(arch, ea, (uint8_t)(rt & 0xFF));
        break;

    case OP_SH:
        cpu_write16(arch, ea, (uint16_t)(rt & 0xFFFF));
        break;

    case OP_SW:
        cpu_write32(arch, ea, rt);
        break;

    default:
        cpu_raise_exception(
            arch,
            CAUSE_ILL_INSTR,
            arch->pc,
            0
        );
        return;
    }

    arch->pc += 4;
}
