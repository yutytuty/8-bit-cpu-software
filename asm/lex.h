#ifndef LEX_H
#define LEX_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cpu.h"

#define REGISTER_NAME_LEN 2

// const char *instruction_strs[16] = {
//   "MOV",
//   "ADD",
//   "SUB",
//   "PUSH",
//   "POP",
//   "LOD",
//   "STO",
//   "",
//   "",
//   "",
//   "",
//   "",
//   "",
//   "",
//   "",
//   "HLT",
// };

// static inline const char* instructionToStr(enum Instruction inst) {
//   return instruction_strs[inst];
// }

static inline enum Instruction strToInstruction(const char *str) {
  if (strlen(str) != 3 && strlen(str) != 4) return (enum Instruction) - 1;

  // 3 letter instructions
  char instructionStr3[4];
  strncpy(instructionStr3, str, 3);
  instructionStr3[3] = 0;
  for (size_t i = 0; i < strlen(instructionStr3); i++)
    instructionStr3[i] = toupper(instructionStr3[i]);

  if (strcmp(instructionStr3, "MOV") == 0)
    return I_MOV;
  else if (strcmp(instructionStr3, "ADD") == 0)
    return I_ADD;
  else if (strcmp(instructionStr3, "SUB") == 0)
    return I_SUB;
  else if (strcmp(instructionStr3, "POP") == 0)
    return I_POP;
  else if (strcmp(instructionStr3, "LOD") == 0)
    return I_LOD;
  else if (strcmp(instructionStr3, "STO") == 0)
    return I_STO;
  else if (strcmp(instructionStr3, "JNZ") == 0)
    return I_JNZ;
  else if (strcmp(instructionStr3, "HLT") == 0)
    return I_HLT;

  // 4 letter instructions
  char instructionStr4[5];
  strncpy(instructionStr4, str, 4);
  instructionStr4[4] = '\0';
  for (size_t i = 0; i < strlen(instructionStr4); i++)
    instructionStr4[i] = toupper(instructionStr4[i]);
  if (strcmp(instructionStr4, "PUSH") == 0) return I_PUSH;
  return (enum Instruction) - 1;
}

static inline enum Register strToRegister(const char *str) {
  // 2 character registers
  if (strlen(str) != 2) return (enum Register) - 1;
  // Make all characters uppcase
  char str_copy[3];
  strcpy(str_copy, str);
  str_copy[2] = '\0';
  for (size_t i = 0; i < strlen(str_copy); i++)
    str_copy[i] = toupper(str_copy[i]);
  if (strcmp(str_copy, "RA") == 0)
    return R_RA;
  else if (strcmp(str_copy, "RB") == 0)
    return R_RB;
  else if (strcmp(str_copy, "RC") == 0)
    return R_RC;
  else if (strcmp(str_copy, "RH") == 0)
    return R_RH;
  else if (strcmp(str_copy, "RL") == 0)
    return R_RL;
  else if (strcmp(str_copy, "RF") == 0)
    return R_RF;

  return (enum Register) - 1;
}

enum ParamType {
  PARAM_REG_REG,
  PARAM_REG_IMM8,
  PARAM_REG_NONE,
  PARAM_IMM8_NONE,
  PARAM_REG_IMM16,
  PARAM_IMM16_NONE,
  PARAM_NONE,
};

struct LineParser {
  char *line;
  size_t line_number;
  enum Instruction inst;
  enum ParamType param_type;
  union {
    union {
      struct {
        enum Register first, second;
      } reg;
      struct {
        enum Register first;
        uint8 second;
      } imm8;
      struct {
        enum Register first;
        uint16 second;
      } imm16;
    } reg_;
    enum Register first_reg;
    uint8 imm8;    // For instructions with one parameter
    uint16 imm16;  // For instructions with one parameter
  } params;
};

enum CritCharType {
  CRIT_REG,
  CRIT_CONST,
};

static inline enum CritCharType goToCriticalCharacter(struct LineParser *ctx) {
  while (isspace(*ctx->line)) {
    ctx->line++;
  }
  if (*ctx->line != '%' && *ctx->line != '$') {
    error("Expected register or constant at %d\n", ctx->line_number);
    exit(EXIT_FAILURE);
  }

  if (*ctx->line == '%') {
    ctx->line++;
    return CRIT_REG;
  }
  if (*ctx->line == '$') {
    ctx->line++;
    return CRIT_CONST;
  }
  error("Unknown character at %d\n", ctx->line_number);
  exit(EXIT_FAILURE);
}

// Sets the line pointer to the start of the line
enum Instruction getLineInstruction(struct LineParser *ctx);

enum Register praseRegister(struct LineParser *ctx);

void parseFirstParam(struct LineParser *ctx);

void parseSecondParam(struct LineParser *ctx);

uint8 evaluateExpression(struct LineParser *ctx);

uint16 evaluateExpression16(struct LineParser *ctx);

#endif
