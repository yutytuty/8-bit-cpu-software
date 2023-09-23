//
// Created by Yotam Yizhar on 20/09/2023.
//

#include "cpu.h"

int main(int argc, char *argv[]) {
  InitLogLevel();
  if (argc != 2) {
    Error("Expected ROM image file parameter");
    printf("Usage: %s <rom_image.img>\n", argv[0]);
    return EXIT_FAILURE;
  }

  std::ifstream image_file;
  image_file.open(argv[1], std::ios::binary);

  Cpu cpu(image_file);
  cpu.Run();

  return EXIT_SUCCESS;
}
