//
// Created by Yotam Yizhar on 21/09/2023.
//

#include "instruction.h"

Instruction GetInstructionType(uint8_t byte) {
  switch (OPCODE(byte)) {
    case 0x0:
      return Instruction::MOV;
    case 0x1:
      return Instruction::ADD;
    case 0x2:
      return Instruction::SUB;
    case 0x3:
      return Instruction::PUSH;
    case 0x4:
      return Instruction::POP;
    case 0x5:
      return Instruction::LOD;
    case 0x6:
      return Instruction::STO;
    case 0x7:
      return Instruction::JNZ;
    case 0xF:
      return Instruction::HLT;
    default:
      Error("Unknown instruction");
      exit(EXIT_FAILURE);
  }
}
