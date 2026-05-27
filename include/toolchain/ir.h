#pragma once
#include <stdint.h>
#include "isa/generated/asm_operands.h"

typedef enum {
    IR_INSTR,      /* инструкция */
    IR_DATA_BYTES, /* .db / .string */
    IR_DATA_WORD,  /* .dw */
    IR_SPACE,      /* .space N  — N нулевых байт */
    IR_ORG,        /* .org ADDR */
    IR_ALIGN,      /* .align N */
    IR_LABEL,      /* определение метки */
    IR_GLOBAL,     /* .global sym */
    IR_EXTERN,     /* .extern sym */
} ir_node_type_t;

typedef enum {
    IOP_REG, IOP_IMM, IOP_LABEL,
} iop_type_t;

typedef struct {
    iop_type_t type;
    union {
        uint32_t reg;
        int64_t  imm;
        char     label[64];
    };
} ir_operand_t;

#define IR_MAX_OPS   8
#define IR_MAX_BYTES 256

typedef struct {
    ir_node_type_t type;
    int            src_line;

    /* IR_INSTR */
    char           mnemonic[32];
    ir_operand_t   ops[IR_MAX_OPS];
    int            nops;

    /* IR_DATA_BYTES / IR_STRING */
    uint8_t        bytes[IR_MAX_BYTES];
    int            nbytes;

    /* IR_DATA_WORD */
    ir_operand_t   word_val; /* может быть IMM или LABEL (релокация) */

    /* IR_SPACE */
    uint32_t       space_size;

    /* IR_ORG / IR_ALIGN */
    uint32_t       org_addr;
    uint32_t       align_val;

    /* IR_LABEL / IR_GLOBAL / IR_EXTERN */
    char           sym_name[64];

    /* заполняется ассемблером на первом проходе */
    uint32_t       addr;   /* адрес этого узла в выходном потоке */
    int            size;   /* размер в байтах */
} ir_node_t;

typedef struct {
    ir_node_t *nodes;
    int        count;
    int        capacity;
} ir_list_t;

void ir_list_init(ir_list_t *l);
void ir_list_free(ir_list_t *l);
ir_node_t *ir_list_push(ir_list_t *l); /* возвращает указатель на новый узел */
