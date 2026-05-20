/*
 * cpu.h
 *
 * Core CPU architecture definitions.
 *
 * Defines:
 *  - CPU execution state
 *  - control registers
 *  - exception model
 *  - interrupt state
 *  - TLB/MMU structures
 *  - memory access interface
 *
 * This module represents the architectural
 * state visible to software and kernel code.
 */

#ifndef CPU_H
#define CPU_H

#include "core/bus.h"

#include <stdint.h>

/*
 * Exception and reset vector address.
 */

#define EXCEPTION_VECTOR  0x1FC00000u

/*
 * Status register flags.
 */

#define SR_IE   (1u << 0)   /* Interrupt Enable          */
#define SR_KM   (1u << 1)   /* Kernel Mode (1=kernel)    */
#define SR_EXL  (1u << 2)   /* Exception Level           */

/*
 * Exception cause codes.
 */

#define CAUSE_INT       0   /* Hardware interrupt        */
#define CAUSE_SYSCALL   8   /* SYSCALL instruction       */
#define CAUSE_ILL_INSTR 10  /* Reserved/illegal instr    */
#define CAUSE_DIV_ZERO  15  /* Division by zero          */
#define CAUSE_TLB_MISS  2   /* TLB miss (load/store)     */
#define CAUSE_BUS_ERROR 7   /* Bus error                 */

/*
 * Number of TLB entries.
 */

#define TLB_ENTRIES 32

/*
 * Translation Lookaside Buffer entry.
 *
 * vpn:
 *      Virtual page number.
 *
 * pfn:
 *      Physical frame number.
 *
 * valid:
 *      Entry validity flag.
 *
 * dirty:
 *      Writable page flag.
 *
 * rwx:
 *      Access permission bits.
 *
 * user:
 *      User-mode accessibility flag.
 */

typedef struct {
    uint32_t vpn;       /* Virtual Page Number  (vaddr >> 12) */
    uint32_t pfn;       /* Physical Frame Number (paddr >> 12) */
    uint8_t  valid;
    uint8_t  dirty;     /* страница была записана               */
    uint8_t  rwx;       /* биты доступа: Read/Write/Execute     */
    uint8_t  user;      /* доступна из user-mode                */
} tlb_entry_t;

/*
 * Complete architectural CPU state.
 *
 * Contains:
 *  - general purpose registers
 *  - control/status registers
 *  - MMU/TLB state
 *  - timer state
 *  - atomic operation state
 *  - execution control flags
 *  - system bus connection
 */

typedef struct {
    uint32_t registers[32];

    uint32_t pc;

    uint32_t sr;        /* Status Register              */
    uint32_t epc;       /* Exception Program Counter    */
    uint32_t cause;     /* Cause Register               */
    uint32_t badvaddr;  /* Bad Virtual Address          */

    uint32_t entry_hi;
    uint32_t entry_lo;
    uint32_t tlb_index; /* Index для TLBR/TLBP          */

    uint64_t mtime;     /* счётчик тактов               */
    uint64_t mtimecmp;  /* порог -> IRQ при mtime>=mtimecmp */

    uint32_t lladdr;    /* адрес последнего LL          */
    uint8_t  ll_valid;  /* флаг валидности              */

    tlb_entry_t tlb[TLB_ENTRIES];
    uint32_t    tlb_random;   /* указатель для TLBWR round-robin */

    bus_t *bus;

    int is_halted;
    int waiting;        /* WFI: ждём прерывания         */
} cpu_t;

/*
 * Control and status register identifiers.
 */

#define CSR_STATUS    0
#define CSR_EPC       1
#define CSR_CAUSE     2
#define CSR_BADVADDR  3
#define CSR_ENTRY_HI  4
#define CSR_ENTRY_LO  5
#define CSR_TLB_INDEX 6
#define CSR_MTIME_LO  7
#define CSR_MTIME_HI  8
#define CSR_MTIMECMP_LO 9
#define CSR_MTIMECMP_HI 10

/*
 * Reset CPU to power-on state.
 */

void     cpu_reset(cpu_t *arch);

/*
 * Execute single instruction cycle.
 */

void     cpu_step(cpu_t *arch);

/*
 * Print complete CPU state for debugging.
 */

void     cpu_dump_state(cpu_t *arch);

/*
 * Read 8-bit value through CPU memory interface.
 */

uint8_t  cpu_read8(cpu_t *arch, uint32_t addr);

/*
 * Read 16-bit value through CPU memory interface.
 */

uint16_t cpu_read16(cpu_t *arch, uint32_t addr);

/*
 * Read 32-bit value through CPU memory interface.
 */

uint32_t cpu_read32(cpu_t *arch, uint32_t addr);

/*
 * Write 8-bit value through CPU memory interface.
 */

void     cpu_write8(cpu_t *arch, uint32_t addr, uint8_t value);

/*
 * Write 16-bit value through CPU memory interface.
 */

void     cpu_write16(cpu_t *arch, uint32_t addr, uint16_t value);

/*
 * Write 32-bit value through CPU memory interface.
 */

void     cpu_write32(cpu_t *arch, uint32_t addr, uint32_t value);

/*
 * Fetch instruction word from instruction stream.
 *
 * Separated from generic data reads to allow
 * future instruction-side MMU/cache logic.
 */

uint32_t cpu_fetch32(cpu_t *arch, uint32_t addr);

/*
 * Read control/status register.
 */

uint32_t csr_read (cpu_t *arch, uint8_t csr);

/*
 * Write control/status register.
 */

void     csr_write(cpu_t *arch, uint8_t csr, uint32_t value);

/*
 * Write TLB entry using replacement policy.
 */

void     tlb_write_random(cpu_t *arch);

/*
 * Read TLB entry into CSR registers.
 */

void     tlb_read        (cpu_t *arch);

/*
 * Search TLB for matching virtual page.
 */

void     tlb_probe       (cpu_t *arch);

/*
 * Invalidate TLB entry by virtual address.
 */

void     tlb_invalidate  (cpu_t *arch, uint32_t vaddr);

/*
 * Translate virtual address into physical address.
 */

uint32_t tlb_translate   (cpu_t *arch, uint32_t vaddr, int write);

#endif
