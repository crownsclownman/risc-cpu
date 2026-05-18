#include "core/bus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void bus_error(uint32_t addr) {
    fprintf(stderr,
            "bus: unmapped access at 0x%08X\n",
            addr);

    exit(1);
}

static void bus_align_error(uint32_t addr) {
    fprintf(stderr,
            "bus: unaligned access at 0x%08X\n",
            addr);

    exit(1);
}

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

void bus_init(bus_t *bus) {
    memset(bus, 0, sizeof(*bus));
}

int bus_attach(bus_t *bus, bus_device_t *dev) {
    if (bus->count >= BUS_MAX_DEVICES)
        return -1;

    bus->devices[bus->count++] = dev;

    return 0;
}

uint8_t bus_read8(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    dev = bus_find(bus, addr);

    if (!dev || !dev->read8)
        bus_error(addr);

    return dev->read8(dev, addr - dev->base);
}

uint16_t bus_read16(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    if (addr & 1)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->read16)
        bus_error(addr);

    return dev->read16(dev, addr - dev->base);
}

uint32_t bus_read32(bus_t *bus, uint32_t addr) {
    bus_device_t *dev;

    if (addr & 3)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->read32)
        bus_error(addr);

    return dev->read32(dev, addr - dev->base);
}

void bus_write8(bus_t *bus, uint32_t addr, uint8_t value) {
    bus_device_t *dev;

    dev = bus_find(bus, addr);

    if (!dev || !dev->write8)
        bus_error(addr);

    dev->write8(dev, addr - dev->base, value);
}

void bus_write16(bus_t *bus, uint32_t addr, uint16_t value) {
    bus_device_t *dev;

    if (addr & 1)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->write16)
        bus_error(addr);

    dev->write16(dev, addr - dev->base, value);
}

void bus_write32(bus_t *bus, uint32_t addr, uint32_t value) {
    bus_device_t *dev;

    if (addr & 3)
        bus_align_error(addr);

    dev = bus_find(bus, addr);

    if (!dev || !dev->write32)
        bus_error(addr);

    dev->write32(dev, addr - dev->base, value);
}
