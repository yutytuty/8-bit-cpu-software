//
// Created by Yotam Yizhar on 20/09/2023.
//

#ifndef INC_8_BIT_CPU_EMU_CPU_H_
#define INC_8_BIT_CPU_EMU_CPU_H_

#include "../common/log.h"
#include "instruction.h"

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <fstream>

#define REGISTER_NUM 7

#define INCORRECT_PARAMETERS_ERROR(opcode) Error("Incorrect parameters for instruction (0x%01X)\n", opcode);\
                                           exit(EXIT_FAILURE);

#define ROM_SIZE 0x8000
#define RAM_SIZE 0x3C00
#define STACK_SIZE 0x300

enum Register {
  RA = 0x0, RB = 0x1, RC = 0x2, RD = 0x3, RH = 0x4, RL = 0x5, RF = 0x6,
};

struct CpuState {
  union Registers {
    struct {
      uint8_t ra_, rb_, rc_, rd_, rh_, rl_, rf_;
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

  uint8_t Peek(uint16_t addr);
  void Poke(uint16_t addr, uint8_t val);

  void Push(uint8_t val);
  uint8_t Pop();
};

class Cpu {
 public:
  bool running_ = false;
  CpuState state_;

  explicit Cpu(std::ifstream &file);

  void SimulateInstruction(Instruction opcode);
  void SimulateInstruction(Instruction opcode, Register reg1, Register reg2);
  void SimulateInstruction(Instruction opcode, Register reg1, uint8_t imm8);
  void SimulateInstruction(Instruction opcode, Register reg);
  void SimulateInstruction(Instruction opcode, uint8_t imm8);
  void SimulateInstruction(Instruction opcode, Register reg, uint16_t imm16);
  void SimulateInstruction(Instruction opcode, uint16_t imm16);
};

#endif //INC_8_BIT_CPU_EMU_CPU_H_
