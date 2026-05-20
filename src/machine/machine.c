/*
 * machine.c
 *
 * Whole-machine initialization and execution layer.
 *
 * Responsibilities:
 *  - initialize all hardware components
 *  - configure system bus and device mappings
 *  - load binary images into RAM or ROM
 *  - reset CPU state
 *  - execute emulation loop
 *
 * This module acts as the top-level integration layer
 * that combines CPU core, memory devices and peripherals
 * into a complete virtual machine.
 */

#include "machine/machine.h"

#include "core/address_space.h"
#include "core/decode.h"
#include "core/execute.h"

#include <stdio.h>
#include <string.h>

/*
 * Load raw binary file into memory buffer.
 *
 * path:
 *      Path to binary image file.
 *
 * dst:
 *      Destination memory buffer.
 *
 * limit:
 *      Maximum allowed binary size.
 *
 * Returns:
 *      0  on success.
 *     -1  on failure.
 *
 * The function verifies that the file size does not
 * exceed the destination memory region size before
 * copying the binary into memory.
 */

static int load_binary(const char *path, uint8_t *dst, uint32_t limit) {
    FILE *f;
    long size;

    f = fopen(path, "rb");

    if (!f)
        return -1;

    fseek(f, 0, SEEK_END);

    size = ftell(f);

    rewind(f);

    if (size < 0 || (uint32_t)size > limit) {
        fclose(f);
        return -1;
    }

    if (fread(dst, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return -1;
    }

    fclose(f);

    return 0;
}

/*
 * Initialize complete virtual machine state.
 *
 * Responsibilities:
 *  - initialize system bus
 *  - initialize RAM device
 *  - initialize ROM device
 *  - initialize UART device
 *  - attach devices to system bus
 *  - reset CPU state
 *  - connect CPU to bus
 *
 * After successful initialization the machine is ready
 * to load binaries and execute code.
 */

void machine_init(machine_t *machine)
{
    bus_init(&machine->bus);
    ram_init(&machine->ram_dev, &machine->ram, RAM_BASE, RAM_SIZE);
    rom_init(&machine->rom_dev, &machine->rom, ROM_BASE, ROM_SIZE);
    uart_init(&machine->uart_dev, &machine->uart, UART_BASE);

    bus_attach(&machine->bus, &machine->ram_dev);
    bus_attach(&machine->bus, &machine->rom_dev);
    bus_attach(&machine->bus, &machine->uart_dev);

    cpu_reset(&machine->cpu);

    machine->cpu.bus = &machine->bus;
}

/*
 * Load binary image into machine memory.
 *
 * machine:
 *      Target machine instance.
 *
 * path:
 *      Path to binary image.
 *
 * addr:
 *      Base address of destination memory region.
 *
 * Returns:
 *      0  on success.
 *     -1  on invalid address or load failure.
 *
 * Supported destinations:
 *  - ROM_BASE
 *  - RAM_BASE
 *
 * The address selects which memory device receives
 * the binary image.
 */

int machine_load_binary(machine_t *machine, const char *path, uint32_t addr) {
    if (addr == ROM_BASE)
        return load_binary(path, machine->rom.data, ROM_SIZE);

    if (addr == RAM_BASE)
        return load_binary(path, machine->ram.data, RAM_SIZE);

    return -1;
}

/*
 * Execute machine until halt or instruction limit.
 *
 * machine:
 *      Target machine instance.
 *
 * max_insns:
 *      Maximum number of instructions to execute.
 *      Zero means unlimited execution.
 *
 * trace:
 *      Enable instruction tracing when non-zero.
 *
 * Execution stops when:
 *  - CPU enters halted state
 *  - instruction limit is reached
 *  - invalid wait state is detected
 *
 * When tracing is enabled, current instruction count
 * and program counter are printed before each step.
 *
 * The main execution loop repeatedly calls cpu_step()
 * until termination conditions are met.
 */

void machine_run(machine_t *machine, uint64_t max_insns, int trace) {
    uint64_t insn_count;

    insn_count = 0;

    while (!machine->cpu.is_halted) {
        if (trace) {
            fprintf(stderr,
                    "[%llu] PC=0x%08X\n",
                    (unsigned long long)insn_count,
                    machine->cpu.pc);
        }

        cpu_step(&machine->cpu);

        if (machine->cpu.waiting) {
            if (!(machine->cpu.sr & SR_IE)) {
                fprintf(stderr, "emu: WFI при IE=0\n");
                break;
            }
            continue;
        }

        insn_count++;

        if (max_insns && insn_count >= max_insns) {
            fprintf(stderr, "emu: instruction limit reached\n");
            break;
        }
    }
}
