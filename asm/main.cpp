//
// Created by Yotam Yizhar on 25/09/2023.
//
#include <iostream>
#include "common/log.h"
#include "lex.h"

int main(int argc, char *argv[]) {
  InitLogLevel();
  if (argc != 2) {
    Error("Expected source file to assemble\n");
    std::cout << "Usage: " << argv[0] << " <source.asm>" << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream file(argv[1]);
  Lexer lex(file);
  lex.GoToStart();
  lex.NextLine();
  Info("Instruction at line %d: %s\n",
       lex.GetLineNum(),
       INSTRUCTION_STRING(lex.GetLineInstruction()));
  return EXIT_SUCCESS;
}
