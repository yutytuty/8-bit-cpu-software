#include <stdio.h>
#include <stdlib.h>

#include "emu.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <image file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Load file into ROM
  const char* filename = argv[1];
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    error("Could not open file %s\n", filename);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

