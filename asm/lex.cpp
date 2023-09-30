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
      return false;
    if ((c != ' ') && (c != '\n'))
      return true;
  }
  return false;
}

Instruction Lexer::GetLineInstruction() {
  // Find start of characters
  const char* sep = " ";
  char* line = const_cast<char *>(line_.c_str());
  while (*line == ' ')
    line++;
  char* token = strtok(line, sep);
  if (token == nullptr) {
    INSTRUCTION_NOT_FOUND_ERROR;
  }
  if (strcmp(token, "MOV") == 0)
    return Instruction::MOV;
  if (strcmp(token, "ADD") == 0)
    return Instruction::ADD;
  if (strcmp(token, "SUB") == 0)
    return Instruction::SUB;
  if (strcmp(token, "PUSH") == 0)
    return Instruction::PUSH;
  if (strcmp(token, "POP") == 0)
    return Instruction::POP;
  if (strcmp(token, "LOD") == 0)
    return Instruction::LOD;
  if (strcmp(token, "STO") == 0)
    return Instruction::STO;
  if (strcmp(token, "JNZ") == 0)
    return Instruction::JNZ;
  if (strcmp(token, "HLT") == 0)
    return Instruction::HLT;
  INSTRUCTION_NOT_FOUND_ERROR;
}

void Lexer::NextLine() {
  getline(file_, line_);
  line_num_++;
}
