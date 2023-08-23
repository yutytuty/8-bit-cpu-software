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

enum InstructionName {
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

static inline uint16 rxhrxl(struct CPU state) {
  return ((state.registers.rxh << 8) | state.registers.rxl);
}

static inline void initialize_cpu(struct CPU* state) {
  state->pc = 0;
}

static inline size_t instruction_length(enum InstructionName inst, int reg_bit) {
  switch (inst) {
    case MOV:
    case ADD:
    case SUB:
      return 2;
    case PUSH:
    case POP:
      return (reg_bit) ? 1 : 2;
    case LOD:
    case STO:
    case JNZ:
      return (reg_bit) ? 1 : 3;
    case HLT:
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

