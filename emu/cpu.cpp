//
// Created by Yotam Yizhar on 20/09/2023.
//

#include "cpu.h"

cpu::cpu(std::ifstream& file) {
  if (!file.is_open()) {
    Error("Could not load image into rom. File is not open\n");
  }

  char byte;
  int index = 0;
  while (file.get(byte)) {
    state_.memory.rom_[index] = byte;
    ++index;
  }
}
uint8_t CpuState::Pop() {
  return memory.stack_[sp--];
}

void CpuState::Push(uint8_t val) {
  memory.stack_[++sp] = val;
}

void CpuState::Poke(uint16_t addr, uint8_t val) {
  memory.raw_[addr] = val;
}

uint8_t CpuState::Peek(uint16_t addr) {
  return memory.raw_[addr];
}
