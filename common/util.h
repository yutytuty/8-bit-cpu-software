#ifndef UTIL_H
#define UTIL_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

typedef uint8_t uint8;
typedef uint16_t uint16;

#define false 0
#define true 1

#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_RESET "\x1b[0m"

static inline void warn(const char *fmt, ...) {
  va_list args;
  printf("%sWARN:", ANSI_YELLOW);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("%s", ANSI_RESET);
}

static inline void error(const char *fmt, ...) {
  va_list args;
  printf("%sERROR: ", ANSI_RED);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("%s", ANSI_RESET);
}

static inline void info(const char *fmt, ...) {
  va_list args;
  printf("%sINFO: ", ANSI_BLUE);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("%s", ANSI_RESET);
}

#endif
