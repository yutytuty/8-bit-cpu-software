#ifndef CPU_H
#define CPU_H

#include "util.h"

// Instruction parsing
#define OPCODE(value) ((value >> 4) & 0xF)
#define REG_BIT(value) ((value >> 3) & 1)
#define SECOND_REG(value) (value >> 6)
#define FIRST_REG(value) (value & 0x3)

#define ROM_SIZE 0x8000
#define RAM_SIZE 0x3C00
#define STCK_SIZE 0x300

// TODO in design document, fill whole memory
#define TOTAL_MEMORY_SIZE ROM_SIZE+RAM_SIZE+STCK_SIZE

enum Instruction {
  MOV  = 0x0,
  ADD  = 0x1,
  SUB  = 0x2,
  PUSH = 0x3,
  POP  = 0x4,
  LOD  = 0x5,
  STO  = 0x6,
  JNZ  = 0x7,
  HLT  = 0xF,
};

struct CPU {
  union {
    struct {
      uint8 ra, rb, rc, rxh, rxl, rf;
    };
    uint8 raw[6];
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

static void inline initialize_cpu(struct CPU* state) {
  state->pc = 0;
}


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

