#include "asm.h"

size_t goToStart(FILE* cursor) {
  int line_number = 0;

  char* line = NULL;
  size_t len = 0;
  // read line
  do {
    getline(&line, &len, cursor);
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
    parseLine(&ctx);
    printf("Instruction: %02x\n", ctx.inst);
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
  FILE *cursor = fopen(argv[1], "r");
  if (cursor == NULL) {
    error("Could not open fd %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  size_t line_number = goToStart(cursor);

  parseLines(cursor, line_number);
  
  fclose(cursor);

  return EXIT_SUCCESS;
}

