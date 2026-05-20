#ifndef EXECUTE_INTERNAL_H
#define EXECUTE_INTERNAL_H

#include "core/cpu.h"
#include "core/decode.h"

#include <stdint.h>

static inline int32_t sign_ext16(uint32_t v) {
    return (int32_t)(int16_t)(uint16_t)v;
}

static inline uint32_t branch_target(cpu_t *arch, int32_t imm) {
    return (arch->pc + 4) + (imm << 2);
}

static inline uint32_t jump_target(cpu_t *arch, uint32_t target) {
    uint32_t base = (arch->pc + 4) & 0xF0000000u;
    return base | ((target & 0x03FFFFFFu) << 2);
}

static inline void reg_write(cpu_t *arch, uint8_t reg, uint32_t value) {
    if (reg != 0)
        arch->registers[reg] = value;
}
void cpu_raise_exception(cpu_t *arch, uint32_t cause);

#endif
