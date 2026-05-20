/*
 * uart.c
 *
 * Minimal MMIO UART device with TX/RX support.
 *
 * Features:
 *  - memory mapped register interface
 *  - transmit support
 *  - receive support
 *  - non-blocking stdin polling
 *  - raw terminal input mode
 *
 * RX model:
 *      Host keyboard input is placed into an internal
 *      receive register and exposed through UART_REG_DATA.
 *
 * TX model:
 *      Bytes written to UART_REG_DATA are immediately
 *      forwarded to host stdout.
 *
 * NOTE:
 *      This UART implementation is synchronous and does
 *      not emulate transmission timing.
 */

#include "devices/uart.h"

#include <stdio.h>
#include <string.h>

static struct termios uart_old_termios;
static int uart_termios_initialized = 0;

static void uart_restore_terminal(void) {
    if (uart_termios_initialized) {
        tcsetattr(STDIN_FILENO, TCSANOW, &uart_old_termios);
    }
}

static void uart_setup_terminal(void) {
    struct termios raw;

    if (uart_termios_initialized)
        return;

    tcgetattr(STDIN_FILENO, &uart_old_termios);

    raw = uart_old_termios;

    raw.c_lflag &= ~(ICANON | ECHO);

    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    atexit(uart_restore_terminal);

    uart_termios_initialized = 1;
}

static void uart_poll_input(uart_t *uart)
{
    unsigned char c;
    ssize_t n;

    if (uart->status & UART_STATUS_RX_READY)
        return;

    n = read(STDIN_FILENO, &c, 1);

    if (n <= 0)
        return;

    uart->rx_data = c;

    uart->status |= UART_STATUS_RX_READY;
}

/*
 * Read 8-bit UART register value.
 *
 * Supported registers:
 *      UART_REG_STATUS
 *
 * Undefined registers return zero.
 */

static uint8_t uart_read8(bus_device_t *dev, uint32_t offset) {
    uart_t *uart;

    uart = (uart_t *)dev->opaque;
    uart_poll_input(uart);

    switch (offset) {

    case UART_REG_STATUS:
        return (uint8_t)uart->status;
    case UART_REG_DATA:
        urat->status &= ~UART_STATUS_RX_READY;
        return uart->rx_data;

    default:
        return 0;
    }
}

/*
 * Read 16-bit UART register value.
 *
 * UART registers are internally byte-sized.
 */

static uint16_t uart_read16(bus_device_t *dev, uint32_t offset) {
    return (uint16_t)uart_read8(dev, offset);
}

/*
 * Read 32-bit UART register value.
 *
 * UART registers are internally byte-sized.
 */

static uint32_t uart_read32(bus_device_t *dev, uint32_t offset) {
    return (uint32_t)uart_read8(dev, offset);
}

/*
 * Write 8-bit UART register value.
 *
 * Writing to UART_REG_DATA transmits a single byte
 * to the host console output.
 */

static void uart_write8(bus_device_t *dev, uint32_t offset, uint8_t value) {
    uart_t *uart;

    uart = (uart_t *)dev->opaque;

    switch (offset) {

    case UART_REG_DATA:

        /*
         * Transmit character to host terminal.
         */        
        putchar((int)value);

        /*
         * Flush stdout to provide immediate console output.
         */
        fflush(stdout);

        /*
         * Mark transmitter as ready after output completes.
         */
        uart->status |= UART_STATUS_TX_READY;

        break;

    default:
        break;
    }
}

/*
 * Write 16-bit UART value.
 *
 * Only the lowest byte is used.
 */

static void uart_write16(bus_device_t *dev, uint32_t offset, uint16_t value) {
    uart_write8(dev, offset, (uint8_t)(value & 0xFF));
}

/*
 * Write 32-bit UART value.
 *
 * Only the lowest byte is used.
 */

static void uart_write32(bus_device_t *dev, uint32_t offset, uint32_t value) {
    uart_write8(dev, offset, (uint8_t)(value & 0xFF));
}

/*
 * Initialize UART device instance.
 *
 * Clears UART state, initializes status flags
 * and configures MMIO bus callbacks.
 *
 * dev:
 *      Bus device descriptor.
 *
 * uart:
 *      UART device state structure.
 *
 * base:
 *      Physical base address of UART MMIO region.
 *
 * The UART occupies a fixed 4 KB MMIO region.
 */

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
