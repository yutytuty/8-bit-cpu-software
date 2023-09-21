//
// Created by Yotam Yizhar on 20/09/2023.
//

#include "Cpu.h"

Cpu::Cpu(std::ifstream& file) {
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
