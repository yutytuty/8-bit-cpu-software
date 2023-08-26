#include "asm.h"

size_t goToStart(FILE* fd) {
  int line_number = 0;

  char* line = NULL;
  size_t len = 0;
  // read line
  do {
    getline(&line, &len, fd);
    line_number++;
    if (line == NULL) {
      error("Read error at line %d\n", line_number);
      exit(1);
    }
  } while (strncmp(line, "start:", strlen("start:")) != 0);
  
  return line_number;
}

void parseLines(FILE* fd, size_t start_line) {
  int line_number = start_line;

  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fd) != -1) {
    line_number++;
    
    struct LineParser ctx;
    ctx.line_number = line_number;
    ctx.line = strdup(line); // Use strdup to simplify memory allocation and copying
    if (ctx.line == NULL) {
      error("Memory allocation failed");
      break; // Exit loop on memory allocation error
    }

    printf("Parsing line number %d\n", line_number);
    enum Instruction inst = getLineInstruction(&ctx);
    printf("Instruction: %02x\n", inst);

    // free(ctx.line); // Free allocated memory for line
  }

  if (feof(fd)) {
    printf("Got to end of file.\n");
  } else if (ferror(fd)) {
    error("Error reading file");
  }

  free(line); // Free allocated memory for getline
}

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <source>", argv[0]);
    return 1;
  }
  FILE *fd = fopen(argv[1], "r");
  if (fd == NULL) {
    error("Could not open fd %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  size_t line_number = goToStart(fd);

  parseLines(fd, line_number);

  
  // enum Instruction inst = getLineInstruction(&ctx);
  // printf("Instruction: %02x\n", inst);
  // parseFirstParam(&ctx);
  // printf("Register: %02x\n", ctx.params.first_reg);
  // parseSecondParam(&ctx);
  // printf("Constant: %02x\n", ctx.params.reg_.imm8.second);

  fclose(fd);

  return EXIT_SUCCESS;
}
