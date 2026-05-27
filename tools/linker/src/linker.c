#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linker.h"

static char lerr[256];

static uint32_t read_u32_le(const uint8_t *buf, uint32_t off) {
    return (uint32_t)buf[off]
        | ((uint32_t)buf[off + 1] << 8)
        | ((uint32_t)buf[off + 2] << 16)
        | ((uint32_t)buf[off + 3] << 24);
}

static void write_u32_le(uint8_t *buf, uint32_t off, uint32_t v) {
    buf[off] = (uint8_t)(v);
    buf[off + 1] = (uint8_t)(v >> 8);
    buf[off + 2] = (uint8_t)(v >> 16);
    buf[off + 3] = (uint8_t)(v >> 24);
}

static int set_error(const char **err_out, const char *msg) {
    snprintf(lerr, sizeof(lerr), "%s", msg);
    *err_out = lerr;
    return -1;
}

static int set_errorf(const char **err_out, const char *fmt, const char *name) {
    snprintf(lerr, sizeof(lerr), fmt, name);
    *err_out = lerr;
    return -1;
}

void linker_init(linker_t *l, uint32_t base_addr) {
    l->objs = NULL;
    l->count = 0;
    l->base_addr = base_addr;
}

void linker_free(linker_t *l) {
    if (l->objs != NULL) {
        free(l->objs);
    }
    l->objs = NULL;
    l->count = 0;
}

int linker_add(linker_t *l, object_t *obj) {
    object_t **new_objs;

    new_objs = (object_t **)realloc(l->objs, (size_t)(l->count + 1) * sizeof(object_t *));
    if (new_objs == NULL) {
        return -1;
    }

    l->objs = new_objs;
    l->objs[l->count] = obj;
    l->count += 1;
    return 0;
}

int linker_link(linker_t *l, const char *out_path, const char **err_out) {
    uint32_t *bases;
    uint32_t cursor;
    uint32_t total;
    uint8_t *out_buf;
    int ngsyms;
    int i;
    int j;
    int k;
    FILE *fp;

    typedef struct {
        char name[TOOLCHAIN_MAX_NAME];
        uint32_t addr;
    } gsym_t;

    gsym_t *gsyms;

    if (l->count <= 0) {
        return set_error(err_out, "no input objects");
    }

    bases = (uint32_t *)calloc((size_t)l->count, sizeof(uint32_t));
    if (bases == NULL) {
        return set_error(err_out, "out of memory");
    }

    cursor = l->base_addr;
    for (i = 0; i < l->count; i++) {
        bases[i] = cursor;
        cursor += l->objs[i]->code_size;
    }
    total = cursor - l->base_addr;

    gsyms = (gsym_t *)calloc((size_t)(l->count * TOOLCHAIN_MAX_SYMBOLS), sizeof(gsym_t));
    if (gsyms == NULL) {
        free(bases);
        return set_error(err_out, "out of memory");
    }

    ngsyms = 0;
    for (i = 0; i < l->count; i++) {
        object_t *o;

        o = l->objs[i];
        for (j = 0; j < o->nsymbols; j++) {
            symbol_t *s;
            int found_dup;

            s = &o->symbols[j];
            if (!s->defined) {
                continue;
            }

            found_dup = 0;
            for (k = 0; k < ngsyms; k++) {
                if (strcmp(gsyms[k].name, s->name) == 0) {
                    found_dup = 1;
                    break;
                }
            }

            if (found_dup) {
                free(gsyms);
                free(bases);
                snprintf(lerr, sizeof(lerr), "duplicate global symbol '%s'", s->name);
                *err_out = lerr;
                return -1;
            }

            strncpy(gsyms[ngsyms].name, s->name, TOOLCHAIN_MAX_NAME - 1);
            gsyms[ngsyms].name[TOOLCHAIN_MAX_NAME - 1] = '\0';
            gsyms[ngsyms].addr = bases[i] + s->value;
            ngsyms += 1;
        }
    }

    if (total == 0) {
        total = 1;
    }

    out_buf = (uint8_t *)calloc((size_t)total, 1);
    if (out_buf == NULL) {
        free(gsyms);
        free(bases);
        return set_error(err_out, "out of memory");
    }

    for (i = 0; i < l->count; i++) {
        object_t *o;
        uint32_t dst_off;

        o = l->objs[i];
        dst_off = bases[i] - l->base_addr;
        if (o->code_size > 0) {
            memcpy(out_buf + dst_off, o->code, o->code_size);
        }
    }

    for (i = 0; i < l->count; i++) {
        object_t *o;
        uint32_t base;

        o = l->objs[i];
        base = bases[i];

        for (j = 0; j < o->nrelocs; j++) {
            reloc_t *r;
            uint32_t sym_addr;
            int found;
            uint32_t patch_off;
            uint32_t instr;
            uint32_t pc;
            int32_t off;

            r = &o->relocs[j];
            found = 0;
            sym_addr = 0;

            for (k = 0; k < ngsyms; k++) {
                if (strcmp(gsyms[k].name, r->sym) == 0) {
                    sym_addr = gsyms[k].addr;
                    found = 1;
                    break;
                }
            }

            if (!found) {
                free(out_buf);
                free(gsyms);
                free(bases);
                snprintf(lerr, sizeof(lerr), "undefined symbol '%s'", r->sym);
                *err_out = lerr;
                return -1;
            }

            patch_off = (base - l->base_addr) + r->offset;
            if (patch_off + 4 > total) {
                free(out_buf);
                free(gsyms);
                free(bases);
                return set_error(err_out, "relocation out of bounds");
            }

            instr = read_u32_le(out_buf, patch_off);

            if (r->type == RELOC_ABS32) {
                write_u32_le(out_buf, patch_off, sym_addr + (uint32_t)r->addend);
                continue;
            }

            if (r->type == RELOC_BRANCH16) {
                pc = base + r->offset + 4;
                off = (int32_t)((int64_t)(sym_addr + (uint32_t)r->addend) - (int64_t)pc) / 4;

                if (off < -32768 || off > 32767) {
                    free(out_buf);
                    free(gsyms);
                    free(bases);
                    snprintf(lerr, sizeof(lerr), "branch relocation out of range for '%s'", r->sym);
                    *err_out = lerr;
                    return -1;
                }

                instr = (instr & 0xFFFF0000u) | (uint32_t)((uint16_t)(int16_t)off);
                write_u32_le(out_buf, patch_off, instr);
                continue;
            }

            if (r->type == RELOC_JTARGET26) {
                uint32_t target;

                target = (sym_addr + (uint32_t)r->addend) / 4;
                if (target > 0x3FFFFFFu) {
                    free(out_buf);
                    free(gsyms);
                    free(bases);
                    snprintf(lerr, sizeof(lerr), "jump relocation out of range for '%s'", r->sym);
                    *err_out = lerr;
                    return -1;
                }

                instr = (instr & 0xFC000000u) | (target & 0x03FFFFFFu);
                write_u32_le(out_buf, patch_off, instr);
                continue;
            }

            free(out_buf);
            free(gsyms);
            free(bases);
            return set_error(err_out, "unknown relocation type");
        }
    }

    fp = fopen(out_path, "wb");
    if (fp == NULL) {
        free(out_buf);
        free(gsyms);
        free(bases);
        snprintf(lerr, sizeof(lerr), "cannot open output '%s'", out_path);
        *err_out = lerr;
        return -1;
    }

    if (total > 0) {
        fwrite(out_buf, 1, (size_t)total, fp);
    }
    fclose(fp);

    free(out_buf);
    free(gsyms);
    free(bases);
    return 0;
}
