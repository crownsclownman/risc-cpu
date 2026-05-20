#include <stdio.h>

#include "assembler/parser.h"

int main(void) {
    const char *source =
        "add r1, r2, r3\n"
        "addi r4, r0, 123\n";

    parser_t parser;
    parsed_instr_t instr;
    int i;

    parser_init(&parser, source);

    while (parser_next_instruction(&parser, &instr) > 0) {
        printf("instr: %s\n", instr.mnemonic);

        for (i = 0; i < instr.operand_count; ++i) {
            printf("  operand %d\n", i);
        }
    }

    return 0;
}
