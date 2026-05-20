/*
 * decode.h
 *
 * Instruction decoding subsystem.
 *
 * Defines internal decoded instruction
 * representation used by execution engine.
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

/*
 * Decoded instruction representation.
 *
 * Stores extracted instruction fields
 * for execution subsystem.
 */

typedef struct decoded_instruction_t {
    uint32_t raw;

    uint32_t opcode;

    uint32_t rs;
    uint32_t rt;
    uint32_t rd;

    uint32_t shamt;
    uint32_t funct;

    uint32_t imm;

    uint32_t target;
} decoded_instruction_t;

/*
 * Decode raw instruction word.
 */

void decode(uint32_t instr, decoded_instruction_t *d);

#endif
