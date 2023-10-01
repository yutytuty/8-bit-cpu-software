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
