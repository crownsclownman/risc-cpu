#ifndef BUS_H
#define BUS_H

#include <stdint.h>

typedef struct bus_device bus_device_t;

struct bus_device
{
    uint32_t base;
    uint32_t size;

    uint8_t  (*read8)(bus_device_t *dev, uint32_t offset);
    uint16_t (*read16)(bus_device_t *dev, uint32_t offset);
    uint32_t (*read32)(bus_device_t *dev, uint32_t offset);

    void (*write8)(bus_device_t *dev,
                   uint32_t offset,
                   uint8_t value);

    void (*write16)(bus_device_t *dev,
                    uint32_t offset,
                    uint16_t value);

    void (*write32)(bus_device_t *dev,
                    uint32_t offset,
                    uint32_t value);

    void *opaque;
};

#define BUS_MAX_DEVICES 32

typedef struct
{
    bus_device_t *devices[BUS_MAX_DEVICES];
    int count;
} bus_t;

void bus_init(bus_t *bus);

int bus_attach(bus_t *bus, bus_device_t *dev);

uint8_t  bus_read8(bus_t *bus, uint32_t addr);
uint16_t bus_read16(bus_t *bus, uint32_t addr);
uint32_t bus_read32(bus_t *bus, uint32_t addr);

void bus_write8(bus_t *bus, uint32_t addr, uint8_t value);
void bus_write16(bus_t *bus, uint32_t addr, uint16_t value);
void bus_write32(bus_t *bus, uint32_t addr, uint32_t value);

#endif
