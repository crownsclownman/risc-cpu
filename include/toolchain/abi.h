#pragma once

#include <stdint.h>

typedef enum
{
    ASM_OP_REG = 0,
    ASM_OP_IMM = 1
} asm_operand_type_t;

typedef enum
{
    ASM_SIG_REG = 0,
    ASM_SIG_IMM = 1,
    ASM_SIG_BRANCH = 2,
    ASM_SIG_JTARGET = 3
} asm_operand_sig_t;

typedef enum
{
    RELOC_ABS32 = 0,     /* абсолютный 32-bit адрес */
    RELOC_BRANCH16 = 1,  /* PC-relative смещение в словах */
    RELOC_JTARGET26 = 2  /* target >> 2 в поле 26 бит */
} reloc_type_t;

#define TOOLCHAIN_MAX_NAME   64
#define TOOLCHAIN_MAX_OPS     8
#define TOOLCHAIN_MAX_BYTES   256
#define TOOLCHAIN_MAX_SYMBOLS 1024
#define TOOLCHAIN_MAX_RELOCS  4096
#define TOOLCHAIN_MAX_CODE    (1u << 20)
