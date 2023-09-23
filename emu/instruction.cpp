//
// Created by Yotam Yizhar on 21/09/2023.
//

#include "instruction.h"

size_t GetInstructionLength(Instruction inst, bool reg_bit) {
  switch (inst) {
    case Instruction::MOV:
    case Instruction::ADD:
    case Instruction::SUB: {
      return 2;
    }
    case Instruction::PUSH: {
      return reg_bit ? 2 : 1;
    }
    case Instruction::POP: {
      return 1;
    }
    case Instruction::LOD:
    case Instruction::STO: {
      return reg_bit ? 3 : 1;
    }
    case Instruction::JNZ: {
      return 3;
    }
    case Instruction::HLT: {
      return 0;
    }
  }
}
