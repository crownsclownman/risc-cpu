#ifndef LEXER_H
#define LEXER_H

#include "assembler/token.h"

typedef struct lexer lexer_t;

struct lexer {
    const char *input;
    int         pos;
    int         line;
};

void lexer_init(lexer_t *lexer, const char *input);
token_t lexer_next(lexer_t *lexer);

#endif
