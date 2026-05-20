#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"

void execute_tlb(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rs;

    rs = arch->registers[d->rs];

    if (d->opcode != OP_SPECIAL) {
        cpu_raise_exception(
            arch,
            CAUSE_ILL_INSTR,
            arch->pc,
            0
        );
        return;
    }

    switch (d->funct) {

    case FUNCT_TLBWR:
        tlb_write_random(arch);
        break;

    case FUNCT_TLBR:
        tlb_read(arch);
        break;

    case FUNCT_TLBP:
        tlb_probe(arch);
        break;

    case FUNCT_TLBINV:
        tlb_invalidate(arch, rs);
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
