#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "assembler/lexer.h"

static char peek(lexer_t *lexer) {
    return lexer->input[lexer->pos];
}

static char advance(lexer_t *lexer) {
    char c;

    c = lexer->input[lexer->pos];

    if (c != '\0') {
        lexer->pos++;
    }

    return c;
}

void lexer_init(lexer_t *lexer, const char *input) {
    lexer->input = input;
    lexer->pos   = 0;
    lexer->line  = 1;
}

static token_t make_token(token_type_t type) {
    token_t tok;

    memset(&tok, 0, sizeof(tok));

    tok.type = type;

    return tok;
}

token_t lexer_next(lexer_t *lexer) {
    token_t tok;
    char c;
    int i;

    while (1) {
        c = peek(lexer);

        /*
         * EOF
         */

        if (c == '\0') {
            tok = make_token(TOKEN_EOF);
            return tok;
        }

        /*
         * Spaces
         */

        if (c == ' ' || c == '\t' || c == '\r') {
            advance(lexer);
            continue;
        }

        /*
         * Newline
         */

        if (c == '\n') {
            advance(lexer);

            lexer->line++;

            tok = make_token(TOKEN_NEWLINE);

            return tok;
        }

        /*
         * Colon
         */

        if (c == ':') {
            advance(lexer);

            tok = make_token(TOKEN_COLON);

            return tok;
        }

        /*
         * Comma
         */

        if (c == ',') {
            advance(lexer);

            tok = make_token(TOKEN_COMMA);

            return tok;
        }

        /*
         * Number
         */

        if (isdigit((unsigned char)c)) {
            tok = make_token(TOKEN_NUMBER);

            i = 0;

            while (isdigit((unsigned char)peek(lexer))) {
                tok.text[i++] = advance(lexer);
            }

            tok.text[i] = '\0';

            tok.value = atoi(tok.text);

            return tok;
        }

        /*
         * Identifier / register
         */

        if (isalpha((unsigned char)c) || c == '_') {
            tok = make_token(TOKEN_IDENT);

            i = 0;

            while (isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_') {
                tok.text[i++] = advance(lexer);
            }

            tok.text[i] = '\0';

            /*
             * Register?
             */

            if (tok.text[0] == 'r' && isdigit((unsigned char)tok.text[1])) {
                tok.type  = TOKEN_REGISTER;
                tok.value = atoi(&tok.text[1]);
            }

            return tok;
        }

        /*
         * Unknown char
         */

        advance(lexer);
    }
}
