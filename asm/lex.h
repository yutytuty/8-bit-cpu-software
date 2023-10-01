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
#include <sstream>

#define START_LABEL "_start:"
#define INSTRUCTION_NOT_FOUND_ERROR Error("Expected instruction at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
#define EXPECTED_CONSTANT_EXPRESSION_ERROR Error("Expected constant at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
#define EXPECTED_REGISTER_ERROR Error("Expected register at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
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
#define REGISTER_CHAR '%'
#define CONSTANT_EXPRESSION_CHAR '$'

enum Instruction {
  MOV = 0x0, ADD = 0x1, SUB = 0x2, PUSH = 0x3, POP = 0x4, LOD = 0x5, STO = 0x6, JNZ = 0x7, HLT = 0xF,
};

enum Register {
  RA = 0x0, RB = 0x1, RC = 0x3, RD = 0x4, RH = 0x5, RL = 0x6, RF = 0x7,
};

class Lexer {
 public:
  explicit Lexer(std::ifstream &file);

  void GoToStart();
  bool IsLineEmpty();
  Instruction GetLineInstruction();

  void TokenizeLine();
  void NextLine();

  Register LexRegister(const std::string &token) const;
  int EvaluateConstant(const std::string &token) const;

  size_t GetLineNum() const;

 private:
  std::ifstream &file_;
  size_t line_num_;
  std::string line_;
  std::vector<std::string> tokens_;
};

#endif //INC_8_BIT_CPU_ASM_LEX_H_
