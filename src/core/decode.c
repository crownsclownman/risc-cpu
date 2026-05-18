#include "core/decode.h"

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
