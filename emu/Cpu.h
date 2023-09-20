//
// Created by Yotam Yizhar on 20/09/2023.
//

#ifndef INC_8_BIT_CPU_EMU_CPU_H_
#define INC_8_BIT_CPU_EMU_CPU_H_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <cstring>

#define REGISTER_NUM 7

#define ROM_SIZE 0x8000
#define RAM_SIZE 0x3C00
#define STACK_SIZE 0x300

enum Register {
  RA = 0x0,
  RB = 0x1,
  RC = 0x2,
  RD = 0x3,
  RH = 0x4,
  RL = 0x5,
  RF = 0x6,
};

enum Instruction {
  MOV = 0x0,
  ADD = 0x1,
  SUB = 0x2,
  PUSH = 0x3,
  POP = 0x4,
  LOD = 0x5,
  STO = 0x6,
  JNZ = 0x7,
  HLT = 0xf,
};

struct CpuState {
  union Registers {
	struct {
	  uint8_t ra_, rb_, rc_, rd_, rh_, rl_, pc_;
	};
	uint8_t raw_[REGISTER_NUM] = {0};
  } registers;

  union Memory {
	struct {
	  uint8_t rom_[ROM_SIZE];
	  uint8_t ram_[RAM_SIZE];
	  uint8_t stack_[STACK_SIZE];
	};
	uint8_t raw_[ROM_SIZE + RAM_SIZE + STACK_SIZE] = {0};
  } memory;

  uint16_t pc = 0;
  uint16_t sp = 0;
};

class Cpu {
 public:
  CpuState state_;

  Cpu();
 private:
};

#endif //INC_8_BIT_CPU_EMU_CPU_H_
