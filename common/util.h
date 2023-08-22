#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

typedef uint8_t uint8;
typedef uint16_t uint16;

#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

static inline void warn(const char *fmt, ...) {
  va_list args;
  printf("%s", ANSI_YELLOW);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("%s", ANSI_RESET);
}

#endif
