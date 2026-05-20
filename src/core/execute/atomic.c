#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"
#include "core/bus.h"

void execute_atomic(cpu_t *arch, decoded_instruction_t *d) {
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

    case OP_LL:
        arch->lladdr   = ea;
        arch->ll_valid = 1;

        reg_write(arch, d->rt, bus_read32(arch->bus, ea));

        break;

    case OP_SC:
        if (arch->ll_valid && arch->lladdr == ea) {
            bus_write32(arch->bus, ea, rt);
            reg_write(arch, d->rt, 1u);
        }
        else {
            reg_write(arch, d->rt, 0u);
        }

        arch->ll_valid = 0;

        break;

    default:
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        return;
    }

    arch->pc += 4;
}
