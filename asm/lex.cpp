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
      Info("Found '%s' at line %d\n", START_LABEL, line_num_);
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

void Lexer::StripToken(std::string& token) {
  while (std::isspace(token[0])) {
    token.erase(token.begin());
  }
  if (token[0] == ';') {
    token.clear();
  }
}

void Lexer::TokenizeLine() {
  std::istringstream iss(line_);
  std::string token;
  iss >> token; // Get instruction (seperated by spaces)
  tokens_.push_back(token);
  if (iss >> token) { // Get First parameter
    StripToken(token);
    tokens_.push_back(token);
  }
  if (std::getline(iss, token, ',')) {
    StripToken(token);
    tokens_.push_back(token); // Get second parameter
  }
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
  tokens_.clear();
  do {
    if (file_.eof())
      return;
    getline(file_, line_);
    line_num_++;
  } while (IsLineEmpty());
  TokenizeLine();
}

int Lexer::EvaluateConstant(const std::string &token) const {
  std::istringstream iss(token);
  char c;
  iss.get(c);
  if (c == CONSTANT_EXPRESSION_CHAR) {
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

Register Lexer::LexRegister(const std::string &token) const {
  std::istringstream iss(token);
  char c;
  iss.get(c);
  if (c != REGISTER_CHAR) {
    EXPECTED_REGISTER_ERROR;
  }

  std::string reg_str = iss.str();
  reg_str.erase(reg_str.begin());
  reg_str = reg_str.substr(0, 2);
  if (reg_str == "ra") {
    return Register::RA;
  }
  if (reg_str == "rb") {
    return Register::RB;
  }
  if (reg_str == "rc") {
    return Register::RC;
  }
  if (reg_str == "rd") {
    return Register::RD;
  }
  if (reg_str == "rh") {
    return Register::RH;
  }
  if (reg_str == "rl") {
    return Register::RL;
  }
  if (reg_str == "rf") {
    return Register::RF;
  }
  if (reg_str == "rhrl") {
    return Register::RHRL;
  }

  EXPECTED_REGISTER_ERROR;
}

void Lexer::ParseLine() {
  Instruction inst = GetLineInstruction();
  bool is_register_inst = IsRegInstruction();
  ParsedLine parsed_line = {};
  parsed_line.instruction = inst;
  Info("Line %d instruction: %s.\n", line_num_, INSTRUCTION_STRING(inst));
  switch (inst) {
    case Instruction::MOV:
    case Instruction::ADD:
    case Instruction::SUB: {
      if (is_register_inst) {
        Register reg1 = LexRegister(tokens_[1]);
        Register reg2 = LexRegister(tokens_[2]);
        parsed_line.params = std::make_tuple(reg1, reg2);
        image_size_ += 2;
        break;
      }
      Register reg1 = LexRegister(tokens_[1]);
      uint8_t imm8 = EvaluateConstant(tokens_[2]);
      parsed_line.params = std::make_tuple(reg1, imm8);
      image_size_ += 2;
      break;
    }
    case Instruction::PUSH: {
      if (is_register_inst) {
        Register reg = LexRegister(tokens_[1]);
        parsed_line.params = reg;
        image_size_ += 1;
        break;
      }
      uint8_t imm8 = EvaluateConstant(tokens_[2]);
      parsed_line.params = imm8;
      image_size_ += 2;
      break;
    }
    case Instruction::POP: {
      Register reg = LexRegister(tokens_[1]);
      parsed_line.params = reg;
      image_size_ += 1;
      break;
    }
    case Instruction::LOD:
    case Instruction::STO: {
      if (is_register_inst) {
        Register reg1 = LexRegister(tokens_[1]);
        parsed_line.params = std::make_tuple(reg1, Register::RHRL);
        image_size_ += 1; // Load always <REG> into RHRL so the second byte is not used
        break;
      }
      Register reg1 = LexRegister(tokens_[1]);
      uint16_t imm16 = EvaluateConstant(tokens_[2]);
      parsed_line.params = std::make_tuple(reg1, imm16);
      image_size_ += 3;
      break;
    }
    case Instruction::JNZ: {
      if (is_register_inst) {
        // Error
      }
      uint16_t imm16 = EvaluateConstant(tokens_[1]);
      parsed_line.params = imm16;
      image_size_ += 3;
      break;
    }
    case Instruction::HLT: {
      image_size_ += 1;
      break;
    }
    default: {
      // Some error, should not happen.
    }
  }
  parsed_lines_.push_back(parsed_line);
}

bool Lexer::GetIsEof() const {
  return file_.eof();
}

bool Lexer::IsRegInstruction() {
  std::string param;
  if (tokens_.size() == 3)
    param = tokens_[2];
  else if (tokens_.size() == 2)
    param = tokens_[1];
  else if (tokens_.size() == 1)
    return false;
  else {
    Error("Expected at least one parameter and no more that 2 for instruction at line %d\n", line_num_);
    exit(EXIT_FAILURE);
  }

  if (param[0] != REGISTER_CHAR && param[0] != CONSTANT_EXPRESSION_CHAR) {
    EXPECTED_REGISTER_OR_CONSTANT_ERROR;
  }
  return param[0] == REGISTER_CHAR;
}
