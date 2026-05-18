#ifndef ROM_H
#define ROM_H

#include "core/bus.h"

#include <stdint.h>

typedef struct {
    uint8_t *data;
    uint32_t size;
} rom_t;

void rom_init(bus_device_t *dev, rom_t *rom, uint32_t base, uint32_t size);

#endif
