#include <string.h>

#include "assembler/parser.h"

static void next(parser_t *parser) {
    parser->current = lexer_next(&parser->lexer);
}

void parser_init(parser_t *parser, const char *input) {
    lexer_init(&parser->lexer, input);

    next(parser);
}

static int is_operand(token_type_t type) {
    return type == TOKEN_REGISTER ||
           type == TOKEN_NUMBER   ||
           type == TOKEN_IDENT;
}

int parser_next_instruction(parser_t *parser, parsed_instr_t *instr) {
    operand_t *op;

    memset(instr, 0, sizeof(*instr));

    /*
     * Skip newlines
     */

    while (parser->current.type == TOKEN_NEWLINE) {
        next(parser);
    }

    /*
     * EOF
     */

    if (parser->current.type == TOKEN_EOF) {
        return 0;
    }

    /*
     * Mnemonic
     */

    if (parser->current.type != TOKEN_IDENT) {
        return -1;
    }

    strcpy(instr->mnemonic, parser->current.text);

    next(parser);

    /*
     * Operands
     */

    while (is_operand(parser->current.type)) {
        op = &instr->operands[instr->operand_count++];

        switch (parser->current.type) {
            case TOKEN_REGISTER:

                op->type  = OPERAND_REG;
                op->value = parser->current.value;

                break;

            case TOKEN_NUMBER:

                op->type  = OPERAND_IMM;
                op->value = parser->current.value;

                break;

            case TOKEN_IDENT:

                op->type = OPERAND_LABEL;
                strcpy(op->text, parser->current.text);

                break;

            default:
                break;
        }

        next(parser);

        if (parser->current.type == TOKEN_COMMA) {
            next(parser);
        }
    }

    return 1;
}
