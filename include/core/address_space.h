#ifndef ADDRESS_SPACE_H
#define ADDRESS_SPACE_H

/*
 * 0x00000000 - 0x0FFFFFFF  RAM   (256 MB)
 * 0x1F000000 - 0x1F00FFFF  UART
 * 0x1F010000 - 0x1F01FFFF  TIMER
 * 0x1F020000 - 0x1F02FFFF  DISK
 * 0x1FC00000 - 0x1FC0FFFF  ROM
 */

#define RAM_BASE        0x00000000u
#define RAM_SIZE        0x10000000u   /* 256 MB */

/* MMIO region is OUTSIDE RAM range */

#define UART_BASE       0x1F000000u
#define UART_SIZE       0x00010000u   /* 64 KB */

#define TIMER_BASE      0x1F010000u
#define TIMER_SIZE      0x00010000u   /* 64 KB */

#define DISK_BASE       0x1F020000u
#define DISK_SIZE       0x00010000u   /* 64 KB */

/* Boot ROM */
#define ROM_BASE        0x1FC00000u
#define ROM_SIZE        0x00010000u   /* 64 KB */

#endif
