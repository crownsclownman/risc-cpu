#include "core/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    EXC_BUS_ERROR = 1,
    EXC_ALIGN     = 2
};

uint8_t ram[RAM_SIZE];
uint8_t rom[ROM_SIZE];

static void raise_exception(int code) {
    fprintf(stderr, "exception: %d\n", code);
    exit(1);
}

static uint8_t uart_read8(uint32_t offset) {
    (void)offset;
    return 0;
}

static void uart_write8(uint32_t offset, uint8_t value) {
    (void)offset;
    (void)value;
}

static uint8_t timer_read8(uint32_t offset) {
    (void)offset;
    return 0;
}

static void timer_write8(uint32_t offset, uint8_t value) {
    (void)offset;
    (void)value;
}

static uint8_t disk_read8(uint32_t offset) {
    (void)offset;
    return 0;
}

static void disk_write8(uint32_t offset, uint8_t value) {
    (void)offset;
    (void)value;
}

static int in_range(uint32_t addr, uint32_t base, uint32_t size) {
    return addr >= base && addr < (base + size);
}

uint8_t mem_read8(uint32_t addr) {
    if (in_range(addr, RAM_BASE, RAM_SIZE)) {
        return ram[addr - RAM_BASE];
    }

    if (in_range(addr, ROM_BASE, ROM_SIZE)) {
        return rom[addr - ROM_BASE];
    }

    if (in_range(addr, UART_BASE, UART_SIZE)) {
        return uart_read8(addr - UART_BASE);
    }

    if (in_range(addr, TIMER_BASE, TIMER_SIZE)) {
        return timer_read8(addr - TIMER_BASE);
    }

    if (in_range(addr, DISK_BASE, DISK_SIZE)) {
        return disk_read8(addr - DISK_BASE);
    }

    raise_exception(EXC_BUS_ERROR);

    return 0;
}

uint16_t mem_read16(uint32_t addr) {
    uint16_t value;

    if (addr & 1) {
        raise_exception(EXC_ALIGN);

        return 0;
    }

    value  = (uint16_t)mem_read8(addr + 0);
    value |= (uint16_t)mem_read8(addr + 1) << 8;

    return value;
}

uint32_t mem_read32(uint32_t addr) {
    uint32_t value;

    if (addr & 3) {
        raise_exception(EXC_ALIGN);

        return 0;
    }

    value  = (uint32_t)mem_read8(addr + 0);
    value |= (uint32_t)mem_read8(addr + 1) << 8;
    value |= (uint32_t)mem_read8(addr + 2) << 16;
    value |= (uint32_t)mem_read8(addr + 3) << 24;

    return value;
}

void mem_write8(uint32_t addr, uint8_t value) {
    if (in_range(addr, RAM_BASE, RAM_SIZE)) {
        ram[addr - RAM_BASE] = value;
        return;
    }

    if (in_range(addr, ROM_BASE, ROM_SIZE)) {
        raise_exception(EXC_BUS_ERROR);
        return;
    }

    if (in_range(addr, UART_BASE, UART_SIZE)) {
        uart_write8(addr - UART_BASE, value);
        return;
    }

    if (in_range(addr, TIMER_BASE, TIMER_SIZE)) {
        timer_write8(addr - TIMER_BASE, value);
        return;
    }

    if (in_range(addr, DISK_BASE, DISK_SIZE)) {
        disk_write8(addr - DISK_BASE, value);
        return;
    }

    raise_exception(EXC_BUS_ERROR);
}

void mem_write16(uint32_t addr, uint16_t value) {
    if (addr & 1) {
        raise_exception(EXC_ALIGN);
        return;
    }

    mem_write8(addr + 0, value & 0xFF);
    mem_write8(addr + 1, (value >> 8) & 0xFF);
}

void mem_write32(uint32_t addr, uint32_t value) {
    if (addr & 3) {
        raise_exception(EXC_ALIGN);
        return;
    }

    mem_write8(addr + 0, value & 0xFF);
    mem_write8(addr + 1, (value >> 8) & 0xFF);
    mem_write8(addr + 2, (value >> 16) & 0xFF);
    mem_write8(addr + 3, (value >> 24) & 0xFF);
}
