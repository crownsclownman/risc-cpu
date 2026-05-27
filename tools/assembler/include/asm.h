#pragma once

#include <stdint.h>
#include "toolchain/object.h"

int assemble_file(const char *path, uint32_t base_addr,
                  object_t *out, const char **err_out);
