/*
 * bus.c
 *
 * System bus implementation.
 *
 * Responsible for:
 *  - device lookup
 *  - MMIO dispatch
 *  - aligned memory accesses
 *  - routing read/write requests to mapped devices
 *
 * The bus acts as an abstraction layer between the CPU
 * and memory mapped devices such as RAM, ROM and UART.
 */

#include "core/bus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Abort execution due to access to an unmapped address.
 */

static void bus_error(uint32_t addr) {
    fprintf(stderr, "bus: unmapped access at 0x%08X\n", addr);
    exit(1);
}

/*
 * Abort execution due to unaligned memory access.
 *
 * 16-bit accesses must be aligned to 2 bytes.
 * 32-bit accesses must be aligned to 4 bytes.
 */

static void bus_align_error(uint32_t addr) {
    fprintf(stderr, "bus: unaligned access at 0x%08X\n", addr);
    exit(1);
}

/*
 * Find mapped device responsible for specified address.
 *
 * Returns:
 *      Pointer to mapped device on success.
 *      NULL if address is unmapped.
 */

static bus_device_t *bus_find(bus_t *bus, uint32_t addr) {
    int i;

    for (i = 0; i < bus->count; i++) {

        bus_device_t *dev = bus->devices[i];

        if (addr >= dev->base &&
            addr < (dev->base + dev->size)) {
            return dev;
        }
    }

    return NULL;
}


/*
 * Initialize bus structure.
 */

void bus_init(bus_t *bus) {
    memset(bus, 0, sizeof(*bus));
}


/*
 * Attach device to system bus.
 *
 * Returns:
 *      0 on success.
 *     -1 if device table is full.
 */

int bus_attach(bus_t *bus, bus_device_t *dev) {
    if (bus->count >= BUS_MAX_DEVICES)
        return -1;

    bus->devices[bus->count++] = dev;

    return 0;
}

/*
 * Read 8-bit value from bus.
 */

uint8_t bus_read8(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    dev = bus_find(bus, addr);

    if (!dev || !dev->read8)
        bus_error(addr);

    return dev->read8(dev, addr - dev->base);
}

/*
 * Read 16-bit value from bus.
 *
 * Generates alignment error on unaligned access.
 */

uint16_t bus_read16(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    if (addr & 1)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->read16)
        bus_error(addr);

    return dev->read16(dev, addr - dev->base);
}

/*
 * Read 32-bit value from bus.
 *
 * Generates alignment error on unaligned access.
 */

uint32_t bus_read32(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    if (addr & 3)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->read32)
        bus_error(addr);

    return dev->read32(dev, addr - dev->base);
}

/*
 * Write 8-bit value to bus.
 */

void bus_write8(bus_t *bus, uint32_t addr, uint8_t value) {
    bus_device_t *dev;

    dev = bus_find(bus, addr);

    if (!dev || !dev->write8)
        bus_error(addr);

    dev->write8(dev, addr - dev->base, value);
}

/*
 * Write 16-bit value to bus.
 *
 * Generates alignment error on unaligned access.
 */

void bus_write16(bus_t *bus, uint32_t addr, uint16_t value) {
    bus_device_t *dev;

    if (addr & 1)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->write16)
        bus_error(addr);

    dev->write16(dev, addr - dev->base, value);
}

/*
 * Write 32-bit value to bus.
 *
 * Generates alignment error on unaligned access.
 */

void bus_write32(bus_t *bus, uint32_t addr, uint32_t value) {
    bus_device_t *dev;

    if (addr & 3)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->write32)
        bus_error(addr);

    dev->write32(dev, addr - dev->base, value);
}
