#ifndef ASM_H
#define ASM_H

#include <stdio.h>
#include <stdlib.h>

#include "../common/util.h"
#include "lex.h"

#define BYTECODE_BLOCK_SIZE 8

struct Assembler {
  FILE *fd;
  uint8* lables;
};

#endif
