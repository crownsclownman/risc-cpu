/*
 * ram.h
 *
 * Physical RAM device implementation.
 *
 * Provides byte-addressable writable memory
 * mapped into system address space.
 */

#ifndef RAM_H
#define RAM_H

#include "core/bus.h"

#include <stdint.h>

/*
 * RAM device state.
 *
 * data:
 *      Raw memory buffer.
 *
 * size:
 *      Total RAM size in bytes.
 */

typedef struct {
    uint8_t *data;
    uint32_t size;
} ram_t;

/*
 * Initialize RAM device.
 *
 * Allocates memory buffer and configures
 * bus device mapping.
 *
 * dev:
 *      Bus device descriptor.
 *
 * ram:
 *      RAM instance.
 *
 * base:
 *      Physical base address.
 *
 * size:
 *      RAM size in bytes.
 */

void ram_init(bus_device_t *dev, ram_t *ram, uint32_t base, uint32_t size);

#endif
