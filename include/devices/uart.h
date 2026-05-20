#ifndef UART_H
#define UART_H

#include "core/bus.h"

#include <stdint.h>

#define UART_REG_DATA   0x00
#define UART_REG_STATUS 0x04

#define UART_STATUS_TX_READY (1u << 0)
#define UART_STATUS_RX_READY (1u << 1)

typedef struct {
    uint32_t status;
    uint8_t rx_data
} uart_t;

void uart_init(bus_device_t *dev, uart_t *uart, uint32_t base);

#endif
