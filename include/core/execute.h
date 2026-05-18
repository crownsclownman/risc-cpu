#ifndef EXECUTE_H
#define EXECUTE_H

#include "core/cpu.h"
#include "core/decode.h"

void execute(cpu_t *arch, decoded_instruction_t *d);

#endif
