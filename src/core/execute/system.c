#include "core/execute.h"
#include "core/execute/internal.h"
#include "core/isa.h"

void execute_system(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t rt;

    rt = arch->registers[d->rt];

    if (d->opcode == OP_SYSTEM) {
        switch (d->rs) {

        case 0x00:
            reg_write(arch, d->rt, csr_read(arch, d->rd));
            break;

        case 0x04:
            csr_write(arch, d->rd, rt);
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

        return;
    }

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

    case FUNCT_SYSCALL:
        cpu_raise_exception(
            arch,
            CAUSE_SYSCALL,
            arch->pc + 4,
            0
        );

        return;

    case FUNCT_ERET:
        arch->sr &= ~SR_EXL;
        arch->sr &= ~SR_KM;
        arch->sr |= SR_IE;

        arch->pc = arch->epc;
        return;
    
    case FUNCT_WFI:
        arch->waiting = 1;
        break;

    case FUNCT_FENCE:
        break;

    case FUNCT_NOP:
        break;

    case FUNCT_HALT:
        arch->is_halted = 1;
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
