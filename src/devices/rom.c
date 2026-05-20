/*
 * rom.c
 *
 * Read-only memory device implementation.
 *
 * This module implements a memory-backed ROM device
 * connected through the system bus interface.
 *
 * Features:
 *  - 8/16/32-bit reads
 *  - write protection
 *  - little-endian memory layout
 *  - dynamically allocated backing storage
 *
 * Write operations are ignored in order to emulate
 * immutable memory.
 *
 * Typical uses:
 *  - boot ROM
 *  - firmware image
 *  - program storage
 */

#include "devices/rom.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Read 8-bit value from ROM.
 */

static uint8_t rom_read8(bus_device_t *dev, uint32_t offset) {
    rom_t *rom;

    rom = (rom_t *)dev->opaque;

    return rom->data[offset];
}

/*
 * Read 16-bit little-endian value from ROM.
 */

static uint16_t rom_read16(bus_device_t *dev, uint32_t offset) {
    rom_t *rom;

    rom = (rom_t *)dev->opaque;

    return ((uint16_t)rom->data[offset + 0] << 0)
         | ((uint16_t)rom->data[offset + 1] << 8);
}

/*
 * Read 32-bit little-endian value from ROM.
 */

static uint32_t rom_read32(bus_device_t *dev, uint32_t offset) {
    rom_t *rom;

    rom = (rom_t *)dev->opaque;

    return ((uint32_t)rom->data[offset + 0] << 0)
         | ((uint32_t)rom->data[offset + 1] << 8)
         | ((uint32_t)rom->data[offset + 2] << 16)
         | ((uint32_t)rom->data[offset + 3] << 24);
}

/*
 * Ignore write access to ROM region.
 *
 * ROM is immutable and does not allow writes.
 */

static void rom_write_ignore(bus_device_t *dev, uint32_t offset, uint32_t value) {
    (void)dev;
    (void)offset;
    (void)value;
}

/*
 * Ignore 8-bit write to ROM.
 */

static void rom_write8(bus_device_t *dev, uint32_t offset, uint8_t value) {
    rom_write_ignore(dev, offset, value);
}

/*
* Ignore 16-bit write to ROM.
*/

static void rom_write16(bus_device_t *dev, uint32_t offset, uint16_t value) {
    rom_write_ignore(dev, offset, value);
}

/*
 * Ignore 32-bit write to ROM.
 */

static void rom_write32(bus_device_t *dev, uint32_t offset, uint32_t value) {
    rom_write_ignore(dev, offset, value);
}

/*
 * Initialize ROM device instance.
 *
 * Allocates backing storage, clears device state
 * and configures bus callbacks.
 *
 * dev:
 *      Bus device descriptor.
 *
 * rom:
 *      ROM device state structure.
 *
 * base:
 *      Physical base address of ROM region.
 *
 * size:
 *      ROM size in bytes.
 *
 * The ROM contents are initially zero-filled.
 *
 * Terminates the emulator on allocation failure.
 */

void rom_init(bus_device_t *dev, rom_t *rom, uint32_t base, uint32_t size) {
    memset(rom, 0, sizeof(*rom));

    rom->size = size;

    rom->data = malloc(size);

    if (!rom->data) {
        fprintf(stderr, "rom: allocation failed\n");
        exit(1);
    }

    memset(rom->data, 0, size);

    dev->base = base;
    dev->size = size;

    dev->read8  = rom_read8;
    dev->read16 = rom_read16;
    dev->read32 = rom_read32;

    dev->write8  = rom_write8;
    dev->write16 = rom_write16;
    dev->write32 = rom_write32;

    dev->opaque = rom;
}
