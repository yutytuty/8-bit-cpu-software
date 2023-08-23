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
  // Run instruction and incrament PC accordingly
  uint8 byte_0 = state->memory.rom[state->pc];
  enum Instruction inst = OPCODE(byte_0);
  int reg_bit = REG_BIT(byte_0);
  int should_continue;
  switch (inst) {
    case MOV: {
      int byte_1 = state->memory.rom[state->pc+1];
      if (reg_bit) {
        // reg mov
        state->registers.raw[FIRST_REG(byte_0)] = state->registers.raw[SECOND_REG(byte_1)];
      } else {
        // imm8 mov
        state->registers.raw[FIRST_REG(byte_0)] = byte_1;
      }
      should_continue = true;
      break;
    }
    case ADD: {
      uint8 byte_1 = state->memory.rom[state->pc+1];
      if (reg_bit) {
        // reg add
        state->registers.raw[FIRST_REG(byte_0)] += state->registers.raw[SECOND_REG(byte_1)];
      } else {
        // imm8 add
        state->registers.raw[FIRST_REG(byte_0)] += byte_1;
      }
      should_continue = true;
      break;
    }
    case SUB: {
      uint8 byte_1 = state->memory.rom[state->pc+1];
      if (reg_bit) {
        state->registers.raw[FIRST_REG(byte_0)] -= state->registers.raw[SECOND_REG(byte_1)];
      } else {
        state->registers.raw[FIRST_REG(byte_0)] -= byte_1;
      }
      should_continue = true;
      break;
    }
    case PUSH: {
      if (reg_bit) {
        push(state, state->registers.raw[FIRST_REG(byte_0)]);
      } else {
        push(state, state->memory.rom[state->pc+1]);
      }
      should_continue = true;
      break;
    }
    case POP: {
      if (reg_bit) {
        state->registers.raw[FIRST_REG(byte_0)] = pop(state);
      } else {
        pop(state);
      }
      should_continue = true;
      break;
    }
    case LOD: {
      if (reg_bit) {
        state->registers.raw[FIRST_REG(byte_0)] = peek(state, rxhrxl(*state));
      } else {
        state->registers.raw[FIRST_REG(byte_0)] = peek16(state, state->pc+1);
      }
      should_continue = true;
      break;
    }
    case STO:
      if (reg_bit) {
        poke(state, rxhrxl(*state), state->registers.raw[FIRST_REG(byte_0)]);
      } else {
        poke(state, peek16(state, state->pc), state->registers.raw[FIRST_REG(byte_0)]);
      }
      should_continue = true;
      break;
    case JNZ:
      if (ZERO_FLAG(state->registers.rf)) {
        if (reg_bit) {
          state->pc = state->registers.raw[FIRST_REG(byte_0)];
        } else {
          state->pc = peek16(state, state->pc+1);
        }
      }
      should_continue = true;
      break;
    case HLT:
      puts("Got HLT");
      should_continue = false;
      break;
    default:
      printf("Unknown instruction %1x\n", inst);
      should_continue = false;
      break;
  }
  state->pc += (inst != JNZ && !ZERO_FLAG(state->registers.rf)) ? instruction_length(inst, reg_bit) : 0;
  return should_continue;
}

static void run(struct CPU* state) {
  printf("Running program\n");
  // Fetch execute cycle
  while (run_instruction(state));
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

