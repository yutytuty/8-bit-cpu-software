//
// Created by Yotam Yizhar on 25/09/2023.
//

#include "lex.h"

Lexer::Lexer(std::ifstream &file) : file_(file) {
  line_num_ = 0;
}

void Lexer::GoToStart() {
  while (!file_.eof()) {
    getline(file_, line_);
    ++line_num_;
    if (line_.rfind(START_LABEL, 0) == 0) {
      Info("Found '%s'\n", START_LABEL);
      return;
    }
  }
}

bool Lexer::IsLineEmpty() {
  for (char c : line_) {
    if (c == ';')
      return true;
    if ((c != ' ') && (c != '\n'))
      return false;
  }
  return true;
}

void Lexer::TokenizeLine() {
  std::istringstream iss(line_);

  for (std::string token; iss >> token;)
    tokens_.push_back(token);
}

Instruction Lexer::GetLineInstruction() {
  // Find start of characters
  if (tokens_.empty()) {
    INSTRUCTION_NOT_FOUND_ERROR;
  }

  std::string token = tokens_[0];

  if (token == "MOV")
    return Instruction::MOV;
  if (token == "ADD")
    return Instruction::ADD;
  if (token == "SUB")
    return Instruction::SUB;
  if (token == "PUSH")
    return Instruction::PUSH;
  if (token == "POP")
    return Instruction::POP;
  if (token == "LOD")
    return Instruction::LOD;
  if (token == "STO")
    return Instruction::STO;
  if (token == "JNZ")
    return Instruction::JNZ;
  if (token == "HLT")
    return Instruction::HLT;

  INSTRUCTION_NOT_FOUND_ERROR;
}

void Lexer::NextLine() {
  do {
    getline(file_, line_);
    line_num_++;
  } while (IsLineEmpty());
  TokenizeLine();
}

size_t Lexer::GetLineNum() const {
  return line_num_;
}

int Lexer::ExpandConstant(const std::string &token) const {
  if (token.rfind(CONSTANT_EXPRESSION_CHAR, 0) != 0) {
    EXPECTED_CONSTANT_EXPRESSION_ERROR;
  }
  std::istringstream iss(token);
  char c;
  iss.get(c);
  if (c == '$') {
    int character_or_expr = iss.peek();
    if (character_or_expr == '(') {
      iss.get(); // take out the '(' character

      int result = 0;
      char op = '+';
      while (iss) {
        int num = 0;
        if (iss >> num) {
          if (num < 0)
            op = '+'; // iss >> num interprets a '-' sign as part of the number;
          switch (op) {
            case '+': {
              result += num;
              break;
            }
            case '*': {
              result *= num;
              break;
            }
            case '/': {
              result /= num;
              break;
            }
            default: {
              Error("Unknown operand at line %d\n", line_num_);
              exit(EXIT_FAILURE);
            }
          }
        } else {
          iss.clear(); // clear error flag
          iss >> op; // read operand

          if (op == ')')
            return result;
        }
      }
    } else {
      int num;
      iss >> num;
      return num;
    }
  }
  EXPECTED_CONSTANT_EXPRESSION_ERROR;
}
