/*
 * cpu.c
 *
 * Core CPU implementation.
 *
 * Responsibilities:
 *  - CPU reset and execution loop
 *  - instruction fetch/decode/execute cycle
 *  - interrupt handling
 *  - CSR access
 *  - TLB management
 *  - memory access helpers
 *  - CPU state debugging
 *
 * This module acts as the central coordination layer
 * between the execution engine, MMU/TLB subsystem,
 * and the system bus.
 *
 *
 *
 * Exception handling model:
 *
 * Exceptions save:
 *  - EPC
 *  - CAUSE
 *  - BADVADDR
 *
 * CPU enters kernel mode and jumps
 * to EXCEPTION_VECTOR.
 */




#include "core/cpu.h"
#include "core/decode.h"
#include "core/execute.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Handle timer interrupts and asynchronous CPU events.
 *
 * Increments MTIME counter and checks MTIMECMP.
 * If interrupts are enabled and timer expired,
 * enters exception mode and jumps to exception vector.
 */

static void cpu_handle_interrupts(cpu_t *arch) {
    arch->mtime++;

    if (arch->mtime >= arch->mtimecmp) {

        if (arch->sr & SR_IE) {

            arch->epc   = arch->pc;
            arch->cause = CAUSE_INT;

            arch->sr |= SR_KM;
            arch->sr &= ~SR_IE;

            arch->pc = EXCEPTION_VECTOR;

            arch->waiting = 0;
        }
    }
}

/*
 * Reset CPU state to power-on defaults.
 *
 * Clears all registers and internal state,
 * initializes control registers,
 * resets TLB replacement index,
 * and sets execution start address
 * to exception/reset vector.
 */

void cpu_reset(cpu_t *arch) {
    memset(arch, 0, sizeof(*arch));

    arch->pc  = EXCEPTION_VECTOR; 
    arch->sr  = SR_KM;

    arch->tlb_random = TLB_ENTRIES - 1;

    arch->mtime    = 0;
    arch->mtimecmp = UINT64_MAX;
}

/*
 * Execute single CPU instruction cycle.
 *
 * Performs:
 *  - instruction fetch
 *  - decode
 *  - execution
 *  - tracing/logging
 *  - interrupt processing
 *
 * This function represents one complete
 * architectural CPU step.
 */

void cpu_step(cpu_t *arch) {
    decoded_instruction_t d;
    uint32_t instr;

    instr = cpu_read32(arch, arch->pc);

    decode(instr, &d);
    execute(arch, &d);

    cpu_handle_interrupts(arch);
}

/*
 * Dump full CPU state to stderr.
 *
 * Prints:
 *  - general purpose registers
 *  - control registers
 *  - exception state
 *  - execution flags
 *
 * Intended for debugging and tracing.
 */

void cpu_dump_state(cpu_t *arch) {
    fprintf(stderr, "\n-- General Purpose Registers --------------------------\n");
    for (int i = 0; i < 32; i++) {
        fprintf(stderr, "  R%-2d  = 0x%08X",
                i, arch->registers[i]);
        if (i % 2 == 1)
            fputc('\n', stderr);
        else
            fprintf(stderr, "    ");
    }
    fprintf(stderr, "\n-- Special Registers -----------------------------------\n");
    fprintf(stderr, "  PC         = 0x%08X\n", arch->pc);
    fprintf(stderr, "  SR         = 0x%08X  (IE=%d KM=%d)\n",
            arch->sr,
            (arch->sr & SR_IE) ? 1 : 0,
            (arch->sr & SR_KM) ? 1 : 0);
    fprintf(stderr, "  EPC        = 0x%08X\n", arch->epc);
    fprintf(stderr, "  CAUSE      = 0x%08X\n", arch->cause);
    fprintf(stderr, "  BADVADDR   = 0x%08X\n", arch->badvaddr);
    fprintf(stderr, "--------------------------------------------------------\n");
}

/*
 * Read 8-bit value from system address space.
 *
 * Access is routed through system bus.
 */

uint8_t cpu_read8(cpu_t *arch, uint32_t addr) {
    return bus_read8(arch->bus, addr);
}

/*
 * Read 16-bit value from system address space.
 */

uint16_t cpu_read16(cpu_t *arch, uint32_t addr) {
    return bus_read16(arch->bus, addr);
}

/*
 * Read 32-bit value from system address space.
 */

uint32_t cpu_read32(cpu_t *arch, uint32_t addr) {
    return bus_read32(arch->bus, addr);
}

/*
 * Write 8-bit value into system address space.
 */

void cpu_write8(cpu_t *arch, uint32_t addr, uint8_t value) {
    bus_write8(arch->bus, addr, value);
}

/*
 * Write 16-bit value into system address space.
 */

void cpu_write16(cpu_t *arch, uint32_t addr, uint16_t value) {
    bus_write16(arch->bus, addr, value);
}

/*
 * Write 32-bit value into system address space.
 */

void cpu_write32(cpu_t *arch, uint32_t addr, uint32_t value) {
    bus_write32(arch->bus, addr, value);
}

/*
 * Fetch 32-bit instruction word.
 *
 * Currently identical to cpu_read32(),
 * but separated to allow future instruction-side
 * MMU/cache/permission handling.
 */

uint32_t cpu_fetch32(cpu_t *arch, uint32_t addr) {
    return cpu_read32(arch, addr);
}

/*
 * Read control and status register.
 *
 * Provides access to privileged CPU state,
 * timer registers, exception registers,
 * and MMU/TLB configuration.
 */

uint32_t csr_read(cpu_t *arch, uint8_t csr) {
    switch (csr) {
    case CSR_STATUS:       return arch->sr;
    case CSR_EPC:          return arch->epc;
    case CSR_CAUSE:        return arch->cause;
    case CSR_BADVADDR:     return arch->badvaddr;
    case CSR_ENTRY_HI:     return arch->entry_hi;
    case CSR_ENTRY_LO:     return arch->entry_lo;
    case CSR_TLB_INDEX:    return arch->tlb_index;
    case CSR_MTIME_LO:     return (uint32_t)(arch->mtime & 0xFFFFFFFFu);
    case CSR_MTIME_HI:     return (uint32_t)(arch->mtime >> 32);
    case CSR_MTIMECMP_LO:  return (uint32_t)(arch->mtimecmp & 0xFFFFFFFFu);
    case CSR_MTIMECMP_HI:  return (uint32_t)(arch->mtimecmp >> 32);
    default:
        fprintf(stderr, "csr_read: неизвестный CSR=%d\n", csr);
        return 0;
    }
}

/*
 * Write control and status register.
 *
 * Some fields are protected or masked
 * depending on privilege semantics.
 */

void csr_write(cpu_t *arch, uint8_t csr, uint32_t value) {
    switch (csr) {
    case CSR_STATUS:
        /* Разрешаем менять только IE и KM - остальное защищено */
        arch->sr = (arch->sr & ~(SR_IE | SR_KM)) | (value & (SR_IE | SR_KM));
        break;
    case CSR_EPC:          arch->epc      = value; break;
    case CSR_CAUSE:        arch->cause    = value; break;
    case CSR_BADVADDR:     /* read-only */          break;
    case CSR_ENTRY_HI:     arch->entry_hi = value; break;
    case CSR_ENTRY_LO:     arch->entry_lo = value; break;
    case CSR_TLB_INDEX:    arch->tlb_index = value & (TLB_ENTRIES - 1); break;
    case CSR_MTIME_LO:
        arch->mtime = (arch->mtime & 0xFFFFFFFF00000000ull) | value;
        break;
    case CSR_MTIME_HI:
        arch->mtime = (arch->mtime & 0x00000000FFFFFFFFull) | ((uint64_t)value << 32);
        break;
    case CSR_MTIMECMP_LO:
        arch->mtimecmp = (arch->mtimecmp & 0xFFFFFFFF00000000ull) | value;
        break;
    case CSR_MTIMECMP_HI:
        arch->mtimecmp = (arch->mtimecmp & 0x00000000FFFFFFFFull) | ((uint64_t)value << 32);
        break;
    default:
        fprintf(stderr, "csr_write: неизвестный CSR=%d\n", csr);
        break;
    }
}

/*
 * Write TLB entry using pseudo-random replacement.
 *
 * Stores current EntryHi/EntryLo state
 * into TLB array using round-robin policy.
 *
 * Entry 0 is treated as wired and skipped.
 */

void tlb_write_random(cpu_t *arch) {
    uint32_t idx = arch->tlb_random;

    arch->tlb[idx].vpn   = arch->entry_hi >> 12;
    arch->tlb[idx].pfn   = arch->entry_lo >> 12;
    arch->tlb[idx].valid = (arch->entry_lo >> 1) & 1;
    arch->tlb[idx].dirty = (arch->entry_lo >> 2) & 1;
    arch->tlb[idx].rwx   = (arch->entry_lo >> 3) & 7;
    arch->tlb[idx].user  = (arch->entry_lo >> 6) & 1;

    /* Двигаем random по кругу, пропуская запись 0 (wired) */
    arch->tlb_random = (idx == 1) ? (TLB_ENTRIES - 1) : (idx - 1);
}

/*
 * Read TLB entry into EntryHi/EntryLo registers.
 *
 * Selected entry is determined by TLB_INDEX CSR.
 */

void tlb_read(cpu_t *arch) {
    uint32_t      idx = arch->tlb_index;
    tlb_entry_t  *e   = &arch->tlb[idx];

    arch->entry_hi = e->vpn << 12;
    arch->entry_lo = (e->pfn  << 12)
                   | (e->valid << 1)
                   | (e->dirty << 2)
                   | ((uint32_t)e->rwx << 3)
                   | ((uint32_t)e->user << 6);
}

/*
 * Probe TLB for virtual page match.
 *
 * Searches TLB for current EntryHi VPN.
 *
 * On success:
 *      TLB_INDEX contains matching index.
 *
 * On failure:
 *      sets bit 31 in TLB_INDEX.
 */

void tlb_probe(cpu_t *arch) {
    uint32_t vpn = arch->entry_hi >> 12;

    for (int i = 0; i < TLB_ENTRIES; i++) {
        if (arch->tlb[i].valid && arch->tlb[i].vpn == vpn) {
            arch->tlb_index = (uint32_t)i;
            return;
        }
    }
    arch->tlb_index = 0x80000000u;  /* не найдено */
}

/*
 * Invalidate TLB entries matching virtual address.
 *
 * Used during:
 *  - page unmapping
 *  - permission updates
 *  - copy-on-write handling
 */

void tlb_invalidate(cpu_t *arch, uint32_t vaddr) {
    uint32_t vpn = vaddr >> 12;

    for (int i = 0; i < TLB_ENTRIES; i++) {
        if (arch->tlb[i].vpn == vpn) {
            arch->tlb[i].valid = 0;
        }
    }
}

/*
 * Translate virtual address into physical address.
 *
 * Performs:
 *  - TLB lookup
 *  - validity checks
 *  - write permission checks
 *  - exception generation
 *
 * write:
 *      Non-zero for store operations.
 *
 * Returns:
 *      Physical address on success.
 *      0 on translation failure.
 *
 * On TLB miss or permission fault:
 *      Updates exception state and enters kernel mode.
 */

uint32_t tlb_translate(cpu_t *arch, uint32_t vaddr, int write) {
    uint32_t vpn = vaddr >> 12;
    uint32_t off = vaddr & 0xFFF;

    for (int i = 0; i < TLB_ENTRIES; i++) {
        tlb_entry_t *e = &arch->tlb[i];

        if (!e->valid || e->vpn != vpn)
            continue;

        /* Проверка прав на запись */
        if (write && !e->dirty) {
            arch->badvaddr = vaddr;
            arch->cause    = CAUSE_TLB_MISS;
            arch->epc      = arch->pc;
            arch->sr      |= SR_KM;
            return 0;
        }

        return (e->pfn << 12) | off;
    }

    /* TLB miss - ядро должно заполнить запись через TLBWR */
    arch->badvaddr = vaddr;
    arch->entry_hi = vaddr & 0xFFFFF000u;
    arch->cause    = CAUSE_TLB_MISS;
    arch->epc      = arch->pc;
    arch->sr      |= SR_KM;
    return 0;
}
