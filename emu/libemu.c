#include "libemu.h"

void push(struct CPU* state, uint8 value) {
  poke(state, ++state->sp, value);
}

void push16(struct CPU* state, uint16 value) {
  uint8 low =  (value & 0xFF),
        high = (value >> 8) & 0xFF;
  poke(state, ++state->sp, value);
}

uint8 pop(struct CPU* state) {
  return state->memory.raw[state->sp--];
}

uint16 pop16(struct CPU* state) {
  return ((state->memory.raw[state->sp--]) << 8) | (state->memory.raw[state->sp--]);
}
