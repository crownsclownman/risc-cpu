#pragma once

#include <stdint.h>
#include "toolchain/object.h"

typedef struct
{
    object_t **objs;
    int        count;
    uint32_t   base_addr;
} linker_t;

void linker_init(linker_t *l, uint32_t base_addr);
void linker_free(linker_t *l);
int  linker_add(linker_t *l, object_t *obj);
int  linker_link(linker_t *l, const char *out_path, const char **err_out);
