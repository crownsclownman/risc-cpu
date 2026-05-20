#ifndef MACHINE_H
#define MACHINE_H

#include "core/cpu.h"
#include "core/bus.h"

#include "devices/ram.h"
#include "devices/rom.h"
#include "devices/uart.h"

typedef struct {
    cpu_t cpu;
    bus_t bus;

    ram_t ram;
    rom_t rom;
    uart_t uart;

    bus_device_t ram_dev;
    bus_device_t rom_dev;
    bus_device_t uart_dev;

} machine_t;

void machine_init(machine_t *machine);

void machine_run(machine_t *machine, uint64_t max_insns, int trace);

int machine_load_binary(machine_t *machine, const char *path, uint32_t addr);

#endif
