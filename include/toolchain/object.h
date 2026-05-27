#pragma once
#include <stdint.h>
#include "ir.h"

typedef enum {
    SYM_LOCAL, SYM_GLOBAL, SYM_EXTERN,
} sym_type_t;

typedef struct {
    char       name[64];
    uint32_t   value;      /* адрес (для EXTERN — 0 до линковки) */
    sym_type_t type;
    int        defined;    /* 1 если определён в этом файле */
} symbol_t;

typedef enum {
    RELOC_ABS32,     /* абсолютный 32-bit адрес (.dw label) */
    RELOC_BRANCH16,  /* PC-relative смещение в словах, 16 бит */
    RELOC_JTARGET26, /* абсолютный адрес в словах, 26 бит */
} reloc_type_t;

typedef struct {
    uint32_t     offset;    /* байтовый offset в секции кода */
    char         sym[64];   /* имя символа */
    reloc_type_t type;
    int32_t      addend;
} reloc_t;

#define MAX_SYMBOLS  1024
#define MAX_RELOCS   4096
#define MAX_CODE     (1<<20)  /* 1 МБ */

typedef struct {
    uint8_t  *code;
    uint32_t  code_size;

    symbol_t  symbols[MAX_SYMBOLS];
    int       nsymbols;

    reloc_t   relocs[MAX_RELOCS];
    int       nrelocs;
} object_t;

object_t *object_new(void);
void      object_free(object_t *o);

symbol_t *object_find_sym(object_t *o, const char *name);
symbol_t *object_add_sym(object_t *o, const char *name,
                          uint32_t value, sym_type_t type, int defined);
reloc_t  *object_add_reloc(object_t *o, uint32_t offset,
                            const char *sym, reloc_type_t type, int32_t addend);
