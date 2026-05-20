/*
 * rom.h
 *
 * Boot ROM device implementation.
 *
 * Provides read-only memory region used
 * for firmware and boot code storage.
 */

#ifndef ROM_H
#define ROM_H

#include "core/bus.h"

#include <stdint.h>

/*
 * ROM device state.
 *
 * data:
 *      ROM image buffer.
 *
 * size:
 *      Total ROM size in bytes.
 */

typedef struct {
    uint8_t *data;
    uint32_t size;
} rom_t;

/*
 * Initialize ROM device.
 *
 * Allocates ROM buffer and configures
 * bus device mapping.
 *
 * dev:
 *      Bus device descriptor.
 *
 * rom:
 *      ROM instance.
 *
 * base:
 *      Physical base address.
 *
 * size:
 *      ROM size in bytes.
 */

void rom_init(bus_device_t *dev, rom_t *rom, uint32_t base, uint32_t size);

#endif
