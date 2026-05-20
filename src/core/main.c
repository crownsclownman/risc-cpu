/*
 * main.c
 *
 * Emulator frontend and program entry point.
 *
 * Responsible for:
 *  - command line parsing
 *  - machine initialization
 *  - binary image loading
 *  - execution configuration
 *  - starting emulator runtime
 *
 * Supports loading binaries into:
 *  - boot ROM
 *  - RAM
 *
 * Also provides:
 *  - execution tracing
 *  - instruction limit control
 *  - custom entry point selection
 */

#include "machine/machine.h"
#include "core/address_space.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Print command line usage information.
 *
 * prog:
 *      Executable name.
 */

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

/*
 * Emulator entry point.
 *
 * Performs:
 *  - CLI argument parsing
 *  - machine initialization
 *  - binary loading
 *  - initial PC configuration
 *  - emulator execution
 *  - final CPU state dump
 *
 * Returns:
 *      0 on success.
 *      Non-zero on failure.
 */

int main(int argc, char **argv) {
    machine_t machine;

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

    machine_init(&machine);

    machine_load_binary(&machine, bin_path, load_addr);

    machine.cpu.pc = start_pc;

    machine_run(&machine, max_insns, trace);

    cpu_dump_state(&machine.cpu);

    return 0;
}
