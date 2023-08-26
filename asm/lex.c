#include "lex.h"

enum Instruction getLineInstruction(struct LineParser *ctx) {
  // Go to first letter
  while (*ctx->line != '\0' && isspace(*ctx->line)) ctx->line++;
  if (*ctx->line == '\0') return -1;
  if (strlen(ctx->line) < 3) return -1;

  char *token = strtok(ctx->line, " ");
  if (token == NULL) return -1;

  ctx->inst = strToInstruction(token);
  // Move until after instruction
  while (*ctx->line != '\0') ctx->line++;
  ctx->line++;
  return ctx->inst;
}

enum Register parseRegister(struct LineParser *ctx) {
  // char register_str[3];
  // strncpy(register_str, ctx->line, 2);
  // register_str[2] = '\0';
  // return strToRegister(register_str);
  ctx->line[2] = '\0';
  const char* base_ptr = ctx->line;
  while (*ctx->line++ != '\0');
  return strToRegister(base_ptr);
}

void parseFirstParam(struct LineParser *ctx) {
  switch (ctx->inst) {
    case I_MOV:
    case I_ADD:
    case I_SUB: {
      enum CritCharType crit_type = goToCriticalCharacter(ctx);
      if (crit_type == CRIT_CONST) {
        error(
            "Expected register as first param, got constant \
at %d\n",
            ctx->line_number);
        exit(EXIT_FAILURE);
      }
      ctx->params.first_reg = parseRegister(ctx);
      return;
    }
    case I_PUSH: {
      enum CritCharType crit_type = goToCriticalCharacter(ctx);
      if (crit_type == CRIT_REG) {
        ctx->param_type = PARAM_REG_NONE;
        ctx->params.first_reg = parseRegister(ctx);
      } else {
        ctx->param_type = PARAM_IMM8_NONE;
        ctx->params.imm8 = evaluateExpression(ctx);
      }
    }
    case I_POP:
      ctx->param_type = PARAM_REG_NONE;
      enum CritCharType crit_type = goToCriticalCharacter(ctx);
      if (crit_type == CRIT_CONST) {
        error(
            "Expected register as first param, got constant \
at %d\n",
            ctx->line_number);
      }
      ctx->params.first_reg = parseRegister(ctx);
    case I_LOD:
    case I_STO:
      goToCriticalCharacter(ctx);
      // ctx->params.first_reg = praseRegister(ctx);  // No idea why this is not
      // working
    case I_JNZ: {
      enum CritCharType crit_type = goToCriticalCharacter(ctx);
      if (crit_type == CRIT_REG) {
        ctx->param_type = PARAM_REG_NONE;
        ctx->params.first_reg = parseRegister(ctx);
      } else {
        ctx->param_type = PARAM_IMM16_NONE;
        ctx->params.imm16 = evaluateExpression16(ctx);
      }
    }
    case I_HLT:
      ctx->param_type = PARAM_NONE;
  }
}

void parseSecondParam(struct LineParser *ctx) {
  switch (ctx->inst) {
    case I_MOV:
    case I_ADD:
    case I_SUB: {
      enum CritCharType crit_type = goToCriticalCharacter(ctx);
      if (crit_type == CRIT_REG) {
        ctx->param_type = PARAM_REG_REG;
        ctx->params.reg_.reg.second = parseRegister(ctx);
      } else {
        ctx->params.reg_.imm8.second = evaluateExpression(ctx);
      }
    }
    case I_LOD:
    case I_STO: {
      while (isspace(*ctx->line) && *ctx->line != '\0') ctx->line++;
      if (*ctx->line == '\0') {
        ctx->param_type = PARAM_REG_NONE;
        return;
      }
      if (*ctx->line != '$') {
        error("Expected constant at %d\n", ctx->line_number);
        return;
      }
      ctx->line++;
      uint16 value = evaluateExpression16(ctx);
      ctx->param_type = PARAM_REG_IMM16;
      ctx->params.reg_.imm16.second = value;
    }
    default:
      error("Unexpected character at line %d\n", ctx->line_number);
  }
}

uint8 evaluateExpression(struct LineParser *ctx) {
  if (*ctx->line == '(') {
    // TODO implement evaluating complex expressions
    return -1;
  }
  char *expression_base_ptr = ctx->line;
  while (*ctx->line != ',' && !isspace(*ctx->line) && *ctx->line != '\0') {
    if (!isdigit(*ctx->line)) {
      error("Invalid number at line %d\n", ctx->line_number);
      exit(EXIT_FAILURE);
    }
    ctx->line++;
  }
  *ctx->line = '\0';
  return atoi(expression_base_ptr);
}

uint16 evaluateExpression16(struct LineParser *ctx) {
  if (*ctx->line == '(') {
    // TODO implement evaluationg complex expressions
    return -1;
  }
  char *expression_base_ptr = ctx->line;
  while (*ctx->line != ',' && !isspace(*ctx->line) && *ctx->line != '\0') {
    if (!isdigit(*ctx->line)) {
      error("Invalid number at line %d\n", ctx->line_number);
      exit(EXIT_FAILURE);
    }
    ctx->line++;
  }
  *ctx->line = '\0';
  return atoi(expression_base_ptr);
}
