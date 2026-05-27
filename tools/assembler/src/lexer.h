#pragma once

#include <stdint.h>

typedef enum {
    TOK_IDENT = 0,
    TOK_LABEL_DEF = 1,
    TOK_DIRECTIVE = 2,
    TOK_REG = 3,
    TOK_IMM = 4,
    TOK_STRING = 5,
    TOK_EOL = 6,
    TOK_ERROR = 7
} tok_type_t;

typedef struct
    tok_type_t type;
    int        line;
    union {
        int64_t imm;
        int     reg;
        char    str[128];
    } u;
} token_t;

typedef struct {
    const char *src;
    int         pos;
    int         line;
} lexer_t;

void    lexer_init(lexer_t *l, const char *src, int start_line);
token_t lexer_next(lexer_t *l);
