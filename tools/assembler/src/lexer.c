#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static const char *g_reg_names[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};

static int parse_reg(const char *s) {
    int i;
    char *end;
    long v;

    if ((s[0] == 'r' || s[0] == 'R') && isdigit((unsigned char)s[1])) {
        v = strtol(s + 1, &end, 10);
        if (*end == '\0' && v >= 0 && v <= 31) {
            return (int)v;
        }
    }

    for (i = 0; i < 32; i++) {
        if (strcmp(s, g_reg_names[i]) == 0) {
            return i;
        }
    }

    return -1;
}

static int unescape(const char *s, char *out, int max) {
    int n;

    n = 0;
    while (*s != '\0' && n < max - 1) {
        if (*s == '\\') {
            s++;
            switch (*s)
            {
            case 'n': out[n++] = '\n'; break;
            case 't': out[n++] = '\t'; break;
            case 'r': out[n++] = '\r'; break;
            case '0': out[n++] = '\0'; break;
            case '\\': out[n++] = '\\'; break;
            case '"': out[n++] = '"'; break;
            default:
                out[n++] = '\\';
                if (*s != '\0' && n < max - 1)
                {
                    out[n++] = *s;
                }
                break;
            }
        }
        else {
            out[n++] = *s;
        }
        s++;
    }

    out[n] = '\0';
    return n;
}

void lexer_init(lexer_t *l, const char *src, int start_line) {
    l->src = src;
    l->pos = 0;
    l->line = start_line;
}

token_t lexer_next(lexer_t *l) {
    token_t t;
    const char *s;
    int p;
    int start;
    int len;
    char word[128];
    char raw[512];
    char *end;
    int64_t v;
    int r;

    memset(&t, 0, sizeof(t));
    t.line = l->line;

    s = l->src;
    p = l->pos;

    while (s[p] != '\0' && s[p] != '\n' && isspace((unsigned char)s[p])) {
        p++;
    }

    if (s[p] == '\0' || s[p] == '\n' || s[p] == ';' || s[p] == '#') {
        if (s[p] == '\n') {
            p++;
        }
        t.type = TOK_EOL;
        l->pos = p;
        return t;
    }

    if (s[p] == '.') {
        p++;
        start = p;
        while (s[p] != '\0' && (isalnum((unsigned char)s[p]) || s[p] == '_')) {
            p++;
        }

        len = p - start;
        if (len <= 0) {
            t.type = TOK_ERROR;
            t.u.str[0] = '.';
            t.u.str[1] = '\0';
            l->pos = p;
            return t;
        }

        if (len > 127) {
            len = 127;
        }

        memcpy(t.u.str, s + start, (size_t)len);
        t.u.str[len] = '\0';
        t.type = TOK_DIRECTIVE;
        l->pos = p;
        return t;
    }

    if (s[p] == '"') {
        p++;
        start = p;
        while (s[p] != '\0' && s[p] != '"' && s[p] != '\n') {
            p++;
        }

        len = p - start;
        if (len > 511) {
            len = 511;
        }

        memcpy(raw, s + start, (size_t)len);
        raw[len] = '\0';

        if (s[p] == '"') {
            p++;
        }

        t.type = TOK_STRING;
        unescape(raw, t.u.str, sizeof(t.u.str));
        l->pos = p;
        return t;
    }

    if (s[p] == '-' || s[p] == '+' || isdigit((unsigned char)s[p])) {
        if ((s[p] == '-' || s[p] == '+') && !isdigit((unsigned char)s[p + 1])) {
            goto ident;
        }

        if (s[p] == '0' && (s[p + 1] == 'b' || s[p + 1] == 'B')) {
            v = strtoll(s + p + 2, &end, 2);
        }
        else {
            v = strtoll(s + p, &end, 0);
        }

        if (end != s + p) {
            t.type = TOK_IMM;
            t.u.imm = v;
            l->pos = (int)(end - s);
            return t;
        }
    }

ident:
    if (isalpha((unsigned char)s[p]) || s[p] == '_') {
        start = p;
        while (s[p] != '\0' && (isalnum((unsigned char)s[p]) || s[p] == '_')) {
            p++;
        }

        len = p - start;
        if (len > 127) {
            len = 127;
        }

        memcpy(word, s + start, (size_t)len);
        word[len] = '\0';

        if (s[p] == ':') {
            p++;
            t.type = TOK_LABEL_DEF;
            memcpy(t.u.str, word, (size_t)len + 1);
            l->pos = p;
            return t;
        }

        r = parse_reg(word);
        if (r >= 0) {
            t.type = TOK_REG;
            t.u.reg = r;
            l->pos = p;
            return t;
        }

        t.type = TOK_IDENT;
        memcpy(t.u.str, word, (size_t)len + 1);
        l->pos = p;
        return t;
    }

    t.type = TOK_ERROR;
    t.u.str[0] = s[p];
    t.u.str[1] = '\0';
    l->pos = p + 1;
    return t;
}
