#ifndef LIBEMU_H
#define LIBEMU_H

#include "../common/util.h"
#include "../common/cpu.h"

void push(struct CPU*, uint8);
void push16(struct CPU*, uint16);

uint8 pop(struct CPU* state);
uint16 pop16(struct CPU* state);

#endif

