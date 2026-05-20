/*
 * machine.h
 *
 * Whole-system integration layer.
 *
 * Defines machine container structure which
 * combines CPU, bus, memory devices,
 * and peripheral devices into a complete system.
 *
 * Responsible for:
 *  - machine initialization
 *  - device attachment
 *  - binary loading
 *  - emulator execution control
 */

#ifndef MACHINE_H
#define MACHINE_H

#include "core/cpu.h"
#include "core/bus.h"

#include "devices/ram.h"
#include "devices/rom.h"
#include "devices/uart.h"

/*
 * Complete emulated machine instance.
 *
 * Contains:
 *  - CPU core
 *  - system bus
 *  - memory devices
 *  - peripheral devices
 *  - bus mapping descriptors
 */

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

/*
 * Initialize complete machine state.
 *
 * Creates CPU, initializes devices,
 * configures memory map,
 * and connects devices to system bus.
 */

void machine_init(machine_t *machine);

/*
 * Start emulator execution loop.
 *
 * machine:
 *      Target machine instance.
 *
 * max_insns:
 *      Maximum instruction count.
 *      Zero means unlimited execution.
 *
 * trace:
 *      Non-zero enables instruction tracing.
 */

void machine_run(machine_t *machine, uint64_t max_insns, int trace);

/*
 * Load flat binary image into machine memory.
 *
 * machine:
 *      Target machine instance.
 *
 * path:
 *      Path to binary image.
 *
 * addr:
 *      Physical load address.
 *
 * Returns:
 *      0 on success.
 *     -1 on failure.
 */

int machine_load_binary(machine_t *machine, const char *path, uint32_t addr);

#endif
