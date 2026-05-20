#ifndef PARSER_H
#define PARSER_H

#include "assembler/lexer.h"

typedef enum operand_type {
    OPERAND_REG,
    OPERAND_IMM,
    OPERAND_LABEL
} operand_type_t;

typedef struct operand operand_t;

struct operand {
    operand_type_t type;
    int            value;
    char           text[64];
};

typedef struct parsed_instr parsed_instr_t;

struct parsed_instr {
    char      mnemonic[64];
    operand_t operands[4];
    int       operand_count;
};

typedef struct parser parser_t;

struct parser {
    lexer_t lexer;
    token_t current;
};

void parser_init(parser_t *parser, const char *input);
int parser_next_instruction(parser_t *parser, parsed_instr_t *instr);

#endif
