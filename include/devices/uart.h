/*
 * uart.h
 *
 * Memory-mapped UART device.
 *
 * Provides simple serial interface
 * for console output and input.
 *
 * Device exposes:
 *  - transmit register
 *  - receive register
 *  - status register
 */

#ifndef UART_H
#define UART_H

#include "core/bus.h"

#include <stdint.h>

/* UART register offsets */
#define UART_REG_DATA   0x00
#define UART_REG_STATUS 0x04

/* UART status flags */
#define UART_STATUS_TX_READY (1u << 0)
#define UART_STATUS_RX_READY (1u << 1)

/*
 * UART device state.
 *
 * status:
 *      Device status flags.
 *
 * rx_data:
 *      Last received byte.
 */

typedef struct {
    uint32_t status;
    uint8_t rx_data;
} uart_t;

/*
 * Initialize UART device.
 *
 * Configures MMIO mapping and
 * initializes UART state.
 *
 * dev:
 *      Bus device descriptor.
 *
 * uart:
 *      UART instance.
 *
 * base:
 *      Physical MMIO base address.
 */

void uart_init(bus_device_t *dev, uart_t *uart, uint32_t base);

#endif
