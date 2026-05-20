/*
 * decode.c
 *
 * Instruction decoder implementation.
 *
 * Converts raw 32-bit instruction words
 * into decoded internal representation
 * used by execution subsystem.
 *
 * Decoder extracts:
 *  - opcode
 *  - register operands
 *  - immediate values
 *  - shift amounts
 *  - function codes
 *  - jump targets
 */

#include "core/decode.h"

/*
 * Decode raw instruction into internal structure.
 *
 * Extracts instruction fields according
 * to ISA encoding format.
 *
 * instr:
 *      Raw 32-bit instruction word.
 *
 * d:
 *      Destination decoded instruction structure.
 */

void decode(uint32_t instr, decoded_instruction_t *d)
{
    d->raw = instr;

    d->opcode = (instr >> 26) & 0x3F;

    d->rs = (instr >> 21) & 0x1F;
    d->rt = (instr >> 16) & 0x1F;
    d->rd = (instr >> 11) & 0x1F;

    d->shamt = (instr >> 6) & 0x1F;
    d->funct = instr & 0x3F;

    d->imm = instr & 0xFFFF;

    d->target = instr & 0x03FFFFFF;
}
