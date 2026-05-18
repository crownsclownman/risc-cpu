#include "core/cpu.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cpu_reset(cpu_t *arch) {
    memset(arch, 0, sizeof(*arch));

    arch->pc  = EXCEPTION_VECTOR; 
    arch->sr  = SR_KM;

    arch->tlb_random = TLB_ENTRIES - 1;

    arch->mtime    = 0;
    arch->mtimecmp = UINT64_MAX;
}

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
 * TLBWR - записать запись по случайному индексу (round-robin).
 * Ядро вызывает при обработке TLB miss.
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
 * TLBR - прочитать запись по tlb_index в EntryHi/EntryLo.
 * Ядро вызывает при munmap / permission change.
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
 * TLBP — найти индекс записи для текущего EntryHi.
 * Если не найдено - устанавливаем бит 31 TLB_INDEX (MIPS-соглашение).
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
 * TLBINV — инвалидировать конкретную запись по виртуальному адресу.
 * Используется при munmap и fork copy-on-write.
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
 * tlb_translate - транслировать виртуальный адрес в физический.
 * write=1 при записи (проверяем dirty/write permission).
 * При промахе - генерируем TLB_MISS исключение и возвращаем 0.
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
