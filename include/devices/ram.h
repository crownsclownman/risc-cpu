#ifndef RAM_H
#define RAM_H

#include "core/bus.h"

#include <stdint.h>

typedef struct {
    uint8_t *data;
    uint32_t size;
} ram_t;

void ram_init(bus_device_t *dev, ram_t *ram, uint32_t base, uint32_t size);

#endif
