#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler/asm.h"
#include "linker.h"
#include "toolchain/object.h"

static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage:\n"
        "  %s -o out.bin file1.s [file2.s ...]\n"
        "  %s -o out.bin --base 0x1000 file1.s file2.s\n",
        prog, prog);
    exit(1);
}

int main(int argc, char **argv)
{
    const char *out_path;
    const char *err;
    linker_t lnk;
    uint32_t base_addr;
    int i;
    int rc;

    out_path = NULL;
    base_addr = 0;
    i = 1;
    rc = 0;
    err = NULL;

    while (i < argc && argv[i][0] == '-')
    {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            out_path = argv[++i];
        }
        else if (strcmp(argv[i], "--base") == 0 && i + 1 < argc)
        {
            base_addr = (uint32_t)strtoul(argv[++i], NULL, 0);
        }
        else
        {
            usage(argv[0]);
        }
        i += 1;
    }

    if (out_path == NULL || i >= argc)
    {
        usage(argv[0]);
    }

    linker_init(&lnk, base_addr);

    for (; i < argc; i++)
    {
        object_t *obj;

        obj = object_new();
        if (obj == NULL)
        {
            fprintf(stderr, "out of memory\n");
            rc = 1;
            break;
        }

        if (assemble_file(argv[i], base_addr, obj, &err) != 0)
        {
            fprintf(stderr, "asm error in '%s': %s\n", argv[i], err);
            object_free(obj);
            rc = 1;
            break;
        }

        if (linker_add(&lnk, obj) != 0)
        {
            fprintf(stderr, "out of memory\n");
            object_free(obj);
            rc = 1;
            break;
        }
    }

    if (rc == 0)
    {
        if (linker_link(&lnk, out_path, &err) != 0)
        {
            fprintf(stderr, "link error: %s\n", err);
            rc = 1;
        }
        else
        {
            printf("linked -> %s\n", out_path);
        }
    }

    for (i = 0; i < lnk.count; i++)
    {
        object_free(lnk.objs[i]);
    }
    linker_free(&lnk);

    return rc;
}
