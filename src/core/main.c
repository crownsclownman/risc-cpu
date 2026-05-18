#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "core/address_space.h"

#include "core/cpu.h"
#include "core/decode.h"
#include "core/execute.h"
#include "core/bus.h"
#include "devices/ram.h"
#include "devices/rom.h"
#include "devices/uart.h"

static int load_binary(const char *path, uint8_t *dst, uint32_t limit) {
    FILE *f;

    f = fopen(path, "rb");

    if (!f) {
        fprintf(stderr,
                "emu: failed to open '%s'\n",
                path);

        return -1;
    }

    fseek(f, 0, SEEK_END);

    long size = ftell(f);

    rewind(f);

    if (size < 0 || (uint32_t)size > limit) {

        fprintf(stderr,
                "emu: file too large\n");

        fclose(f);

        return -1;
    }

    if (fread(dst, 1, (size_t)size, f)
        != (size_t)size) {

        fclose(f);

        return -1;
    }

    fclose(f);

    return 0;
}

static void usage(const char *prog) {
    fprintf(stderr,
        "Использование:\n"
        "  %s [опции] <binary>\n"
        "\n"
        "Опции:\n"
        "  -rom <file>   загрузить файл в Boot ROM (0x1FC00000) [по умолчанию]\n"
        "  -ram <file>   загрузить файл в RAM (0x00000000)\n"
        "  -pc  <hex>    начальный PC (по умолчанию: 0x1FC00000)\n"
        "  -max <n>      лимит инструкций (0 = без лимита)\n"
        "  -trace        печатать PC каждой инструкции\n"
        "\n"
        "Примеры:\n"
        "  %s -rom boot.bin\n"
        "  %s -ram kernel.bin -pc 0x00000000\n",
        prog, prog, prog);
}

int main(int argc, char **argv) {
    const char *bin_path  = NULL;
    uint32_t    load_addr = ROM_BASE;
    uint32_t    load_limit= ROM_SIZE;
    uint32_t    start_pc  = EXCEPTION_VECTOR;   /* 0x1FC00000 */
    uint64_t    max_insns = 0;                  /* 0 = без лимита */
    int         trace     = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-rom") == 0 && i + 1 < argc) {
            bin_path   = argv[++i];
            load_addr  = ROM_BASE;
            load_limit = ROM_SIZE;
        } else if (strcmp(argv[i], "-ram") == 0 && i + 1 < argc) {
            bin_path   = argv[++i];
            load_addr  = RAM_BASE;
            load_limit = RAM_SIZE;
        } else if (strcmp(argv[i], "-pc") == 0 && i + 1 < argc) {
            start_pc = (uint32_t)strtoul(argv[++i], NULL, 16);
        } else if (strcmp(argv[i], "-max") == 0 && i + 1 < argc) {
            max_insns = (uint64_t)strtoull(argv[++i], NULL, 10);
        } else if (strcmp(argv[i], "-trace") == 0) {
            trace = 1;
        } else if (argv[i][0] != '-' && bin_path == NULL) {
            bin_path = argv[i];
        } else {
            fprintf(stderr, "emu: неизвестный аргумент '%s'\n\n", argv[i]);
            usage(argv[0]);
            return 1;
        }
    }

    if (!bin_path) {
        usage(argv[0]);
        return 1;
    }

    cpu_t cpu;
    bus_t bus;

    bus_device_t ram_dev;
    bus_device_t rom_dev;
    bus_device_t uart_dev;

    ram_t ram;
    rom_t rom;
    uart_t uart;

    bus_init(&bus);
    ram_init(&ram_dev, &ram, RAM_BASE, RAM_SIZE);
    rom_init(&rom_dev, &rom, ROM_BASE, ROM_SIZE);
    uart_init(&uart_dev, &uart, UART_BASE, UART_SIZE);

    bus_attach(&bus, &ram_dev);
    bus_attach(&bus, &rom_dev);
    bus_attach(&bus, &uart_dev);

    cpu_reset(&cpu);
    cpu.bus = &bus;
    
    if (load_addr == ROM_BASE) {
        load_binary(bin_path, rom.data, ROM_SIZE);
    } else {
        load_binary(bin_path, ram.data, RAM_SIZE);
    }

    cpu.pc = start_pc;

    uint64_t insn_count = 0;
    decoded_instruction_t d;

    while (!cpu.is_halted) {

        if (trace)
            fprintf(stderr, "[%llu] PC=0x%08X\n",
                    (unsigned long long)insn_count, cpu.pc);

        uint32_t instr = bus_read32(cpu.bus, cpu.pc);

        decode(instr, &d);

        execute(&cpu, &d);

        cpu.mtime++;
        if (cpu.mtime >= cpu.mtimecmp) {
            if (cpu.sr & SR_IE) {
                cpu.epc    = cpu.pc;
                cpu.cause  = CAUSE_INT;
                cpu.sr    |= SR_KM;
                cpu.sr    &= ~SR_IE;
                cpu.pc     = EXCEPTION_VECTOR;
                cpu.waiting = 0;
            }
        }

        if (cpu.waiting) {
            if (!(cpu.sr & SR_IE)) {
                fprintf(stderr, "emu: WFI при IE=0 — бесконечное ожидание, halt\n");
                break;
            }
            continue;   /* не инкрементируем счётчик инструкций */
        }

        insn_count++;

        if (max_insns && insn_count >= max_insns) {
            fprintf(stderr, "emu: достигнут лимит %llu инструкций\n",
                    (unsigned long long)max_insns);
            break;
        }
    }

    fprintf(stderr,
            "emu: остановка на PC=0x%08X  инструкций выполнено: %llu\n",
            cpu.pc, (unsigned long long)insn_count);

    cpu_dump_state(&cpu);

    return 0;
}
