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
  if (!bytes_read) {
    error("Could not read file\n");
  }
}

static int run_instruction(struct CPU* state) {
  uint8 byte_0 = state->memory.rom[state->pc];
  enum Instruction inst = OPCODE(byte_0);
  switch (inst) {
    case MOV: {
      int byte_1 = state->memory.rom[state->pc+1];
      if (REG_BIT(byte_0)) {
        // reg mov
        state->registers.raw[FIRST_REG(byte_0)] = state->registers.raw[SECOND_REG(byte_1)];
      } else {
        // imm8 mov
        state->registers.raw[FIRST_REG(byte_0)] = byte_1;
      }
      return true;
    }
    case HLT:
      puts("Got HLT instruction");
      return false;
    default:
      printf("instruction %1x ", inst);
      puts("hit default");
      return false;
  }
}

static void run(struct CPU* state) {
  printf("Running program\n");
  // Fetch execute cycle
  while (run_instruction(state)) {
    // TODO make this dependent on the previous instruction
    state->pc += 2;
  }
  puts("================");
  printf("Registers: %02x %02x %02x %02x %02x %02x\n", state->registers.ra, state->registers.rb, state->registers.rc, state->registers.rxh, state->registers.rxl, state->registers.rf);
}

int main(int argc, char *argv[]) {
  struct CPU state;
  initialize_cpu(&state);

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

  printf("=== IMAGE ===\n");
  for (int i = 0; i < 5; ++i) {
    printf("%02x ", state.memory.rom[i]);
  }
  printf("\n=============\n");

  printf("Loaded program into memory\n");

  run(&state);

  return EXIT_SUCCESS;
}

