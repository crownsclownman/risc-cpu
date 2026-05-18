#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
/*
    Physical memory map:

    0x00000000 - 0x0FFFFFFF   RAM        (256 MB)
    0x1F000000 - 0x1F00FFFF   UART       (MMIO)
    0x1F001000 - 0x1F001FFF   TIMER      (MMIO)
    0x1F002000 - 0x1F002FFF   DISK       (MMIO)
    0x1FC00000 - 0x1FC0FFFF   BOOT ROM   (64 KB)
*/

#define RAM_BASE        0x00000000u
#define RAM_SIZE        0x10000000u

#define UART_BASE       0x1F000000u
#define UART_SIZE       0x00010000u

#define TIMER_BASE      0x1F001000u
#define TIMER_SIZE      0x00001000u

#define DISK_BASE       0x1F002000u
#define DISK_SIZE       0x00001000u

#define ROM_BASE        0x1FC00000u
#define ROM_SIZE        0x00010000u

extern uint8_t ram[RAM_SIZE];
extern uint8_t rom[ROM_SIZE];

uint8_t  mem_read8(uint32_t addr);
uint16_t mem_read16(uint32_t addr);
uint32_t mem_read32(uint32_t addr);

void mem_write8(uint32_t addr, uint8_t value);
void mem_write16(uint32_t addr, uint16_t value);
void mem_write32(uint32_t addr, uint32_t value);


#endif
