#include "devices/uart.h"

#include <stdio.h>
#include <string.h>

static uint8_t uart_read8(bus_device_t *dev, uint32_t offset) {
    uart_t *uart;

    uart = (uart_t *)dev->opaque;

    switch (offset) {

    case UART_REG_STATUS:
        return (uint8_t)uart->status;

    default:
        return 0;
    }
}

static uint16_t uart_read16(bus_device_t *dev, uint32_t offset) {
    return (uint16_t)uart_read8(dev, offset);
}

static uint32_t uart_read32(bus_device_t *dev, uint32_t offset) {
    return (uint32_t)uart_read8(dev, offset);
}

static void uart_write8(bus_device_t *dev, uint32_t offset, uint8_t value) {
    uart_t *uart;

    uart = (uart_t *)dev->opaque;

    switch (offset) {

    case UART_REG_DATA:

        putchar((int)value);

        fflush(stdout);

        uart->status |= UART_STATUS_TX_READY;

        break;

    default:
        break;
    }
}

static void uart_write16(bus_device_t *dev, uint32_t offset, uint16_t value) {
    uart_write8(dev, offset, (uint8_t)(value & 0xFF));
}

static void uart_write32(bus_device_t *dev, uint32_t offset, uint32_t value) {
    uart_write8(dev, offset, (uint8_t)(value & 0xFF));
}

void uart_init(bus_device_t *dev, uart_t *uart, uint32_t base) {
    memset(uart, 0, sizeof(*uart));

    uart->status = UART_STATUS_TX_READY;

    dev->base = base;
    dev->size = 0x1000;

    dev->read8  = uart_read8;
    dev->read16 = uart_read16;
    dev->read32 = uart_read32;

    dev->write8  = uart_write8;
    dev->write16 = uart_write16;
    dev->write32 = uart_write32;

    dev->opaque = uart;
}
