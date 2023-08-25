#ifndef CPU_H
#define CPU_H

#include "util.h"

// TODO in design document, fill whole memory
#define TOTAL_MEMORY_SIZE ROM_SIZE+RAM_SIZE+STCK_SIZE

// Instruction parsing
#define OPCODE(value) ((value >> 4) & 0xF)
#define REG_BIT(value) ((value >> 3) & 1)
#define SECOND_REG(value) (value >> 6)
#define FIRST_REG(value) (value & 0x3)

// Get flags
#define ZERO_FLAG(flag_reg) (flag_reg & 0x1)
#define CARRY_FLAG(flag_reg) ((flag_reg >> 1) & 0x1)

#define ROM_SIZE 0x8000
#define RAM_SIZE 0x3C00
#define STCK_SIZE 0x300

enum Instruction : uint8 {
  I_MOV  = 0x0,
  I_ADD  = 0x1,
  I_SUB  = 0x2,
  I_PUSH = 0x3,
  I_POP  = 0x4,
  I_LOD  = 0x5,
  I_STO  = 0x6,
  I_JNZ  = 0x7,
  I_HLT  = 0xF,
};

enum Register : uint8 {
  R_RA = 0x0,
  R_RB = 0x1,
  R_RC = 0x2,
  R_RH = 0x3,
  R_RL = 0x4,
  R_RF = 0x5,
};

struct CPU {
  union {
    struct {
      uint8 ra, rb, rc, rh, rl, rf;
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

static inline uint16 rhrl(struct CPU state) {
  return ((state.registers.rh << 8) | state.registers.rl);
}

static inline void initialize_cpu(struct CPU* state) {
  state->pc = 0;
}

static inline size_t instruction_length(enum Instruction inst, int reg_bit) {
  switch (inst) {
    case I_MOV:
    case I_ADD:
    case I_SUB:
      return 2;
    case I_PUSH:
    case I_POP:
      return (reg_bit) ? 1 : 2;
    case I_LOD:
    case I_STO:
    case I_JNZ:
      return (reg_bit) ? 1 : 3;
    case I_HLT:
      return 0;
  }
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

static inline uint16 peek16(struct CPU* state, uint16 addr) {
  if (addr > TOTAL_MEMORY_SIZE - 1) {
    warn("Attempted accessing out of bounds memory: %04x\n", addr);
    return 0x0;
  }
  return *(uint16*)(&state->memory.raw[addr]);
}

#endif
