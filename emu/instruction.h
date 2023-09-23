//
// Created by Yotam Yizhar on 21/09/2023.
//

#ifndef INC_8_BIT_CPU_EMU_INSTRUCTION_H_
#define INC_8_BIT_CPU_EMU_INSTRUCTION_H_

#include "../common/log.h"
#include <iostream>

#define OPCODE(byte)  (byte>>4)
#define REG_BIT(byte) ((byte>>3)&0x1)
#define REG_1(byte)   (byte&0x7)
#define REG_2(byte)   (byte>>5)

enum Instruction {
  MOV = 0x0,
  ADD = 0x1,
  SUB = 0x2,
  PUSH = 0x3,
  POP = 0x4,
  LOD = 0x5,
  STO = 0x6,
  JNZ = 0x7,
  HLT = 0xF,
};

size_t GetInstructionLength(Instruction inst, bool reg_bit);

#endif //INC_8_BIT_CPU_EMU_INSTRUCTION_H_
