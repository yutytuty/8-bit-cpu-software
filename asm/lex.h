//
// Created by Yotam Yizhar on 25/09/2023.
//

#ifndef INC_8_BIT_CPU_ASM_LEX_H_
#define INC_8_BIT_CPU_ASM_LEX_H_

#include "../common/log.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#define START_LABEL "_start:"
#define INSTRUCTION_NOT_FOUND_ERROR Error("Expected instruction at line %d\n", line_num_); \
        exit(EXIT_FAILURE);
#define INSTRUCTION_STRING(index) \
        ((index == 0) ? "MOV" : \
        (index == 1) ? "ADD" : \
        (index == 2) ? "SUB" : \
        (index == 3) ? "PUSH" : \
        (index == 4) ? "POP" : \
        (index == 5) ? "LOD" : \
        (index == 6) ? "STO" : \
        (index == 7) ? "JNZ" : \
        (index == 0xF) ? "HLT" : \
        "")

enum Instruction {
  MOV = 0x0, ADD = 0x1, SUB = 0x2, PUSH = 0x3, POP = 0x4, LOD = 0x5, STO = 0x6, JNZ = 0x7, HLT = 0xF,
};

class Lexer {
 public:
  explicit Lexer(std::ifstream &file);

  void GoToStart();
  bool IsLineEmpty();
  Instruction GetLineInstruction();

  void NextLine();

 private:
  std::ifstream &file_;
  size_t line_num_;
  std::string line_;
};

#endif //INC_8_BIT_CPU_ASM_LEX_H_
