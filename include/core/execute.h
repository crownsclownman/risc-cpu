#ifndef EXECUTE_H
#define EXECUTE_H

#include "core/cpu.h"
#include "core/decode.h"

void execute(cpu_t *arch, decoded_instruction_t *d);

void execute_special_alu(cpu_t *arch, decoded_instruction_t *d);
void execute_immediate_alu(cpu_t *arch, decoded_instruction_t *d);
void execute_branch(cpu_t *arch, decoded_instruction_t *d);
void execute_jump(cpu_t *arch, decoded_instruction_t *d);
void execute_loadstore(cpu_t *arch, decoded_instruction_t *d);
void execute_atomic(cpu_t *arch, decoded_instruction_t *d);
void execute_system(cpu_t *arch, decoded_instruction_t *d);
void execute_tlb(cpu_t *arch, decoded_instruction_t *d);

#endif
