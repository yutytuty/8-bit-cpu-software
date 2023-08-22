#include <stdio.h>
#include <stdlib.h>

#include "emu.h"

static void load_image(struct CPU* state, FILE* image) {
  // Determine file size
  fseek(image, 0, SEEK_END);
  long image_size = ftell(image);
  fseek(image, 0, SEEK_SET);
  if (image_size > ROM_SIZE) {
    warn("Image to large for ROM. last %d bytes not loaded.", image_size - ROM_SIZE);
    image_size = ROM_SIZE;
  }
  // Read the file into rom
  unsigned long bytes_read = fread(state->memory.rom, 1, image_size, image);
}

int main(int argc, char *argv[]) {
  struct CPU state;

  if (argc != 2) {
    printf("Usage: %s <image file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Load file into ROM
  const char* filename = argv[1];
  FILE* image = fopen(filename, "rb");
  if (image == NULL) {
    error("Could not open file %s\n", filename);
    return EXIT_FAILURE;
  }
  load_image(&state, image);

  return EXIT_SUCCESS;
}

