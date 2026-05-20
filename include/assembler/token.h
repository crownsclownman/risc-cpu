#ifndef TOKEN_H
#define TOKEN_H

typedef enum token_type {
    TOKEN_EOF = 0,
    TOKEN_IDENT,
    TOKEN_NUMBER,
    TOKEN_REGISTER,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_NEWLINE
} token_type_t;

typedef struct token token_t;

struct token {
    token_type_t type;
    char         text[64];
    int          value;
    int          line;
};

#endif
