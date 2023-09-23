//
// Created by Yotam Yizhar on 20/09/2023.
//

#include "cpu.h"

Cpu::Cpu(std::ifstream &file) {
  if (!file.is_open()) {
    Error("Could not load image into rom. File is not open\n");
  }

  char byte;
  int index = 0;
  while (file.get(byte)) {
    state_.memory.rom_[index] = byte;
    ++index;
  }
  running_ = true;
}
void Cpu::SimulateInstruction(Instruction opcode) {
  if (!running_)
    return;
  switch (opcode) {
    case Instruction::HLT: {
      running_ = false;
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, Register reg1, Register reg2) {
  if (!running_)
    return;
  uint8_t *reg_a = &state_.registers.raw_[reg1];
  uint8_t reg_b = state_.registers.raw_[reg2];
  switch (opcode) {
    case Instruction::MOV: {
      *reg_a = reg_b;
      break;
    }
    case Instruction::ADD: {
      *reg_a += reg_b;
      break;
    }
    case Instruction::SUB: {
      *reg_a -= reg_b;
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, Register reg1, uint8_t imm8) {
  if (!running_)
    return;
  uint8_t *reg_1 = &state_.registers.raw_[reg1];
  switch (opcode) {
    case Instruction::MOV: {
      *reg_1 = imm8;
      break;
    }
    case Instruction::ADD: {
      *reg_1 += imm8;
      break;
    }
    case Instruction::SUB: {
      *reg_1 -= imm8;
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, Register reg) {
  if (!running_)
    return;
  switch (opcode) {
    case Instruction::PUSH: {
      state_.Push(state_.registers.raw_[reg]);
      break;
    }
    case Instruction::POP: {
      state_.registers.raw_[reg] = state_.Pop();
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, uint8_t imm8) {
  if (!running_)
    return;
  switch (opcode) {
    case Instruction::PUSH: {
      state_.Push(imm8);
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, Register reg, uint16_t imm16) {
  if (!running_)
    return;
  uint8_t *p_reg = &state_.registers.raw_[reg];
  switch (opcode) {
    case Instruction::LOD: {
      *p_reg = state_.Peek(imm16);
      break;
    }
    case Instruction::STO: {
      state_.Poke(imm16, *p_reg);
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

void Cpu::SimulateInstruction(Instruction opcode, uint16_t imm16) {
  if (!running_)
    return;
  switch (opcode) {
    case Instruction::JNZ: {
      state_.pc = imm16;
      break;
    }
    default: {
      INCORRECT_PARAMETERS_ERROR(opcode);
    }
  }
}

size_t Cpu::ParseInstruction(const uint8_t *instruction_start) {
  switch (OPCODE(*instruction_start)) {
    case Instruction::MOV:
    case Instruction::ADD:
    case Instruction::SUB: {
      if (REG_BIT(*instruction_start)) {
        SimulateInstruction((Instruction)OPCODE(*instruction_start),
                            (Register)REG_1(*instruction_start),
                            (Register)REG_2(*(instruction_start + 1)));
        break;
      }

      SimulateInstruction((Instruction)OPCODE(*instruction_start),
                          (Register)REG_1(*instruction_start),
                          (uint8_t)(*(instruction_start + 1)));
      break;
    }
    case Instruction::PUSH: {
      if (REG_BIT(*instruction_start)) {
        SimulateInstruction((Instruction)OPCODE(*instruction_start),
                            (Register)REG_1(*instruction_start));
        break;
      }
      SimulateInstruction((Instruction)OPCODE(*instruction_start),
                          (uint8_t)(*(instruction_start + 1)));
      break;
    }
    case Instruction::POP: {
      SimulateInstruction((Instruction)OPCODE(*instruction_start),
                          (Register)REG_1(*instruction_start));
      break;
    }
    case Instruction::LOD:
    case Instruction::STO: {
      if (REG_BIT(*instruction_start)) {
        // TODO: add support for RHRL
        break;
      }
      SimulateInstruction((Instruction)OPCODE(*instruction_start),
                          (Register)REG_1(*instruction_start),
                          (uint16_t)(*(instruction_start + 1)));
      break;
    }
    case Instruction::JNZ: {
      SimulateInstruction((Instruction)OPCODE(*instruction_start),
                          (uint16_t)(*(instruction_start + 1)));
      break;
    }
    case Instruction::HLT: {
      SimulateInstruction((Instruction)OPCODE(*instruction_start));
      Info("Got HLT. Stopping.");
      break;
    }
    default: {
      Error("Unknown instruction (0x%01X)\n", OPCODE(*instruction_start));
      exit(EXIT_FAILURE);
    }
  }
  return GetInstructionLength((Instruction)OPCODE(*instruction_start), REG_BIT(*instruction_start));
}

void Cpu::Run() {
  do
    state_.pc += ParseInstruction(&state_.memory.raw_[state_.pc]);
  while (running_);
}

uint8_t CpuState::Pop() {
  return memory.stack_[sp--];
}

void CpuState::Push(uint8_t val) {
  memory.stack_[++sp] = val;
}

void CpuState::Poke(uint16_t addr, uint8_t val) {
  if (addr < ROM_SIZE) {
    Error("Tried writing to ROM memory at address %01X\n", addr);
    exit(EXIT_FAILURE);
  }
  memory.raw_[addr] = val;
}

uint8_t CpuState::Peek(uint16_t addr) {
  return memory.raw_[addr];
}
