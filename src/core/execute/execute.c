/*
 * execute.c
 *
 * Central instruction execution dispatcher.
 *
 * Routes decoded instructions to
 * specialized execution subsystems:
 *
 *  - ALU operations
 *  - branches and jumps
 *  - memory access
 *  - atomic operations
 *  - system instructions
 *  - TLB management
 *
 * Also responsible for:
 *  - illegal instruction handling
 *  - default PC increment logic
 *  - exception entry coordination
 */

#include "core/execute.h"
#include "core/isa.h"
#include "core/execute/internal.h"

#include <stdio.h>

/*
 * Execute decoded instruction.
 *
 * Dispatches instruction based on opcode
 * and function fields.
 *
 * If instruction does not explicitly modify PC,
 * execution advances to next sequential instruction.
 */

void execute(cpu_t *arch, decoded_instruction_t *d) {
    uint32_t old_pc;

    old_pc = arch->pc;

    switch (d->opcode) {

    case OP_SPECIAL:
        switch (d->funct) {

        case FUNCT_ADD:
        case FUNCT_SUB:
        case FUNCT_AND:
        case FUNCT_OR:
        case FUNCT_XOR:
        case FUNCT_NOR:
        case FUNCT_SLT:
        case FUNCT_SLTU:

        case FUNCT_SLL:
        case FUNCT_SRL:
        case FUNCT_SRA:
        case FUNCT_SLLV:
        case FUNCT_SRLV:
        case FUNCT_SRAV:

        case FUNCT_MUL:
        case FUNCT_MULH:
        case FUNCT_DIV:
        case FUNCT_DIVU:
        case FUNCT_REM:
        case FUNCT_REMU:
            execute_special_alu(arch, d);
            break;

        case FUNCT_JR:
        case FUNCT_JALR:
            execute_jump(arch, d);
            break;

        case FUNCT_SYSCALL:
        case FUNCT_ERET:
        case FUNCT_WFI:
        case FUNCT_HALT:
        case FUNCT_NOP:
        case FUNCT_FENCE:
            execute_system(arch, d);
            break;

        case FUNCT_TLBWR:
        case FUNCT_TLBR:
        case FUNCT_TLBP:
        case FUNCT_TLBINV:
            execute_tlb(arch, d);
            break;

        default:
            cpu_raise_exception(arch, CAUSE_ILL_INSTR);
            break;
        }
        break;
    case OP_ADDI:
    case OP_SLTI:
    case OP_SLTIU:
    case OP_ANDI:
    case OP_ORI:
    case OP_XORI:
        execute_immediate_alu(arch, d);
        break;

    case OP_BEQ:
    case OP_BNE:
    case OP_BLT:
    case OP_BGE:
    case OP_BLTU:
    case OP_BGEU:
        execute_branch(arch, d);
        break;

    case OP_J:
    case OP_JAL:
        execute_jump(arch, d);
        break;

    case OP_LB:
    case OP_LH:
    case OP_LW:
    case OP_LBU:
    case OP_LHU:
    case OP_SB:
    case OP_SH:
    case OP_SW:
        execute_loadstore(arch, d);
        break;

    case OP_LL:
    case OP_SC:
        execute_atomic(arch, d);
        break;

    case OP_SYSTEM:
        execute_system(arch, d);
        break;

    default:
        fprintf(stderr, "illegal instruction: opcode=0x%02X funct=0x%02X\n", d->opcode, d->funct);
        cpu_raise_exception(arch, CAUSE_ILL_INSTR);
        break;
    }

    /*
     * Advance PC automatically if instruction
     * did not modify control flow explicitly.
     *
     * Branch/jump/exception handlers are expected
     * to update PC themselves.
     */

    if (!arch->is_halted && arch->pc == old_pc) {
        arch->pc += 4;
    }
}

/*
 * Enter CPU exception handler.
 *
 * Saves current execution state,
 * disables interrupts,
 * switches CPU into kernel mode,
 * and transfers execution to exception vector.
 *
 * cause:
 *      Architecture-defined exception code.
 */

void cpu_raise_exception(cpu_t *arch, uint32_t cause) {
    arch->cause = cause;
    arch->epc   = arch->pc;

    arch->sr |= SR_KM;
    arch->sr &= ~SR_IE;

    arch->pc = EXCEPTION_VECTOR;
}
