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
#include <variant>
#include <optional>

#define START_LABEL "_start:"
#define INSTRUCTION_NOT_FOUND_ERROR Error("Expected instruction at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
#define EXPECTED_CONSTANT_EXPRESSION_ERROR Error("Expected constant at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
#define EXPECTED_REGISTER_ERROR Error("Expected register at line %d\n", line_num_); \
        exit(EXIT_FAILURE)
#define EXPECTED_REGISTER_OR_CONSTANT_ERROR Error("Expected register or constant at line %d\n", line_num_); \
                                            exit(EXIT_FAILURE)
#define INSTRUCTION_STRING(inst) \
        ((inst == 0) ? "MOV" : \
        (inst == 1) ? "ADD" : \
        (inst == 2) ? "SUB" : \
        (inst == 3) ? "PUSH" : \
        (inst == 4) ? "POP" : \
        (inst == 5) ? "LOD" : \
        (inst == 6) ? "STO" : \
        (inst == 7) ? "JNZ" : \
        (inst == 0xF) ? "HLT" : \
        "")
#define REGISTER_CHAR '%'
#define CONSTANT_EXPRESSION_CHAR '$'

enum Instruction {
  MOV = 0x0, ADD = 0x1, SUB = 0x2, PUSH = 0x3, POP = 0x4, LOD = 0x5, STO = 0x6, JNZ = 0x7, HLT = 0xF,
};

// RHRL is a special register, in the binary code it will not show up but is needed for parsing
enum Register {
  RA = 0x0, RB = 0x1, RC = 0x3, RD = 0x4, RH = 0x5, RL = 0x6, RF = 0x7, RHRL = 0xFF,
};

struct ParsedLine {
  Instruction instruction;
  std::optional<std::variant<
      std::tuple<Register, Register>,
      std::tuple<Register, uint8_t>,
      Register,
      uint8_t,
      std::tuple<Register, uint16_t>,
      uint16_t
  >> params;
};

class Lexer {
 public:
  explicit Lexer(std::ifstream &file);

  void GoToStart();
  bool IsLineEmpty();
  Instruction GetLineInstruction();

  void TokenizeLine();
  void NextLine();

  [[nodiscard]] Register LexRegister(const std::string &token) const;
  [[nodiscard]] int EvaluateConstant(const std::string &token) const;

  void ParseLine();

  [[nodiscard]] bool GetIsEof() const;

 private:
  bool IsRegInstruction(); // If not, REG_BIT should not be set.
  static void StripToken(std::string& token);

  std::ifstream &file_;
  size_t line_num_;
  std::string line_;
  std::vector<std::string> tokens_;

  std::vector<ParsedLine> parse_lines_;
};

#endif //INC_8_BIT_CPU_ASM_LEX_H_
