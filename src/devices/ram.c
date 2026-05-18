#include "devices/ram.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint8_t ram_read8(bus_device_t *dev, uint32_t offset) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    return ram->data[offset];
}

static uint16_t ram_read16(bus_device_t *dev, uint32_t offset) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    return ((uint16_t)ram->data[offset + 0] << 0)
         | ((uint16_t)ram->data[offset + 1] << 8);
}

static uint32_t ram_read32(bus_device_t *dev, uint32_t offset) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    return ((uint32_t)ram->data[offset + 0] << 0)
         | ((uint32_t)ram->data[offset + 1] << 8)
         | ((uint32_t)ram->data[offset + 2] << 16)
         | ((uint32_t)ram->data[offset + 3] << 24);
}

static void ram_write8(bus_device_t *dev, uint32_t offset, uint8_t value) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    ram->data[offset] = value;
}

static void ram_write16(bus_device_t *dev, uint32_t offset, uint16_t value) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    ram->data[offset + 0] = value & 0xFF;
    ram->data[offset + 1] = (value >> 8) & 0xFF;
}

static void ram_write32(bus_device_t *dev, uint32_t offset, uint32_t value) {
    ram_t *ram;

    ram = (ram_t *)dev->opaque;

    ram->data[offset + 0] = value & 0xFF;
    ram->data[offset + 1] = (value >> 8) & 0xFF;
    ram->data[offset + 2] = (value >> 16) & 0xFF;
    ram->data[offset + 3] = (value >> 24) & 0xFF;
}

void ram_init(bus_device_t *dev, ram_t *ram, uint32_t base, uint32_t size) {
    memset(ram, 0, sizeof(*ram));

    ram->size = size;

    ram->data = malloc(size);

    if (!ram->data) {
        fprintf(stderr, "ram: allocation failed\n");
        exit(1);
    }

    memset(ram->data, 0, size);

    dev->base = base;
    dev->size = size;

    dev->read8  = ram_read8;
    dev->read16 = ram_read16;
    dev->read32 = ram_read32;

    dev->write8  = ram_write8;
    dev->write16 = ram_write16;
    dev->write32 = ram_write32;

    dev->opaque = ram;
}
