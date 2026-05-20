/*
 * bus.h
 *
 * System bus and MMIO abstraction layer.
 *
 * Provides:
 *  - device registration
 *  - memory mapped I/O routing
 *  - unified memory access interface
 *
 * All physical memory and peripherals are
 * accessed through this subsystem.
 */

#ifndef BUS_H
#define BUS_H

#include <stdint.h>

/*
 * Memory mapped bus device descriptor.
 *
 * base:
 *      Physical base address.
 *
 * size:
 *      Device address space size in bytes.
 *
 * read/write:
 *      Device access callbacks.
 *
 * opaque:
 *      Pointer to device-specific state.
 */

typedef struct bus_device bus_device_t;

struct bus_device {
    uint32_t base;
    uint32_t size;

    uint8_t  (*read8)(bus_device_t *dev, uint32_t offset);
    uint16_t (*read16)(bus_device_t *dev, uint32_t offset);
    uint32_t (*read32)(bus_device_t *dev, uint32_t offset);

    void (*write8)(bus_device_t *dev, uint32_t offset, uint8_t value);
    void (*write16)(bus_device_t *dev, uint32_t offset, uint16_t value);
    void (*write32)(bus_device_t *dev, uint32_t offset, uint32_t value);

    void *opaque;
};

/*
 * Maximum number of devices attached
 * to system bus.
 */

#define BUS_MAX_DEVICES 32

/*
 * System bus state.
 *
 * devices:
 *      Attached MMIO devices.
 *
 * count:
 *      Number of registered devices.
 */

typedef struct {
    bus_device_t *devices[BUS_MAX_DEVICES];
    int count;
} bus_t;

/*
 * Initialize system bus state.
 */

void bus_init(bus_t *bus);

/*
 * Attach device to system bus.
 *
 * dev:
 *      Device descriptor.
 *
 * Returns:
 *      0 on success.
 *     -1 if bus is full.
 */

int bus_attach(bus_t *bus, bus_device_t *dev);

/*
 * Read 8-bit value from physical address space.
 *
 * Access is routed to matching MMIO device.
 */

uint8_t  bus_read8(bus_t *bus, uint32_t addr);

/*
 * Read 16-bit value from physical address space.
 */

uint16_t bus_read16(bus_t *bus, uint32_t addr);

/*
 * Read 32-bit value from physical address space.
 */

uint32_t bus_read32(bus_t *bus, uint32_t addr);

/*
 * Write 8-bit value into physical address space.
 */

void bus_write8(bus_t *bus, uint32_t addr, uint8_t value);

/*
 * Write 16-bit value into physical address space.
 */

void bus_write16(bus_t *bus, uint32_t addr, uint16_t value);

/*
 * Write 32-bit value into physical address space.
 */

void bus_write32(bus_t *bus, uint32_t addr, uint32_t value);

#endif
