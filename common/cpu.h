#ifndef CPU_H
#define CPU_H

#include "util.h"

#define ROM_SIZE 0x8000
#define RAM_SIZE 0x3C00
#define STCK_SIZE 0x300

// TODO in design document, fill whole memory
#define TOTAL_MEMORY_SIZE ROM_SIZE+RAM_SIZE+STCK_SIZE

struct CPU {
  struct Registers {
    uint8_t ra, rb, rc, rxh, rxl, rf;
  } registers;

  union {
    struct {
      uint8 rom[ROM_SIZE];
      uint8 ram[RAM_SIZE];
      uint8 stck[STCK_SIZE];
    };

    uint8 raw[ROM_SIZE + RAM_SIZE + STCK_SIZE];
  } memory;

  uint16_t sp;
  uint16_t pc;
};

static inline void poke(struct CPU* state, uint16 addr, uint8 value) {
  if (addr > TOTAL_MEMORY_SIZE) {
    warn("Attempted accessing out of bounds memory: %04x\n", addr);
    return;
  }
  if (addr < ROM_SIZE) {
    warn("Attempting to write to read only memory: %04x\n", addr);
    return;
  }
  state->memory.raw[addr] = value;
}

static inline uint8 peek(struct CPU* state, uint16 addr) {
  if (addr > TOTAL_MEMORY_SIZE) {
    warn("Attempted accessing out of bounds memory: %04x\n", addr);
    return 0x0;
  }
  return state->memory.raw[addr];
}

#endif
