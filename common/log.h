//
// Created by Yotam Yizhar on 20/09/2023.
//

#ifndef INC_8_BIT_CPU_COMMON_LOG_H_
#define INC_8_BIT_CPU_COMMON_LOG_H_

#include <iostream>
#include <cstdarg>
#include <typeinfo>
#include <string>

#define ANSI_BOLD_RED     "\x1b[1;31m"
#define ANSI_BOLD_GREEN   "\x1b[1;32m"
#define ANSI_BOLD_YELLOW  "\x1b[1;33m"
#define ANSI_BOLD_MAGENTA "\x1b[1;35m"
#define ANSI_BOLD_CYAN    "\x1b[1;36m"
#define ANSI_RESET        "\x1b[0m"

enum LogLevel {
  NONE = 0, SUCCESS = 1, ERROR = 2, WARN = 3, INFO = 4, DEBUG = 5,
};

extern LogLevel log_level;

constexpr char kLogLevelKey[] = "LOG_LEVEL";

inline void InitLogLevel() {
  const char *content = std::getenv(kLogLevelKey);
  if (content == nullptr) {
    log_level = LogLevel::NONE;
  }
  switch (std::stoi(content)) {
    case 1: {
      log_level = SUCCESS;
      break;
    }
    case 2: {
      log_level = ERROR;
      break;
    }
    case 3: {
      log_level = WARN;
      break;
    }
    case 4: {
      log_level = INFO;
      break;
    }
    case 5: {
      log_level = DEBUG;
      break;
    }
  }
}

inline void Success(const char *fmt, ...) {
  if (log_level < LogLevel::SUCCESS) {
    return;
  }
  va_list args;
  printf("%s[Success]%s ", ANSI_BOLD_GREEN, ANSI_RESET);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

inline void Error(const char *fmt, ...) {
  if (log_level < LogLevel::ERROR) {
    return;
  }
  va_list args;
  printf("%s[Error]%s ", ANSI_BOLD_RED, ANSI_RESET);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

inline void Warn(const char *fmt, ...) {
  if (log_level < LogLevel::WARN) {
    return;
  }
  va_list args;
  printf("%s[Warn]%s ", ANSI_BOLD_YELLOW, ANSI_RESET);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

inline void Info(const char *fmt, ...) {
  if (log_level < LogLevel::INFO) {
    return;
  }
  va_list args;
  printf("%s[Info]%s ", ANSI_BOLD_CYAN, ANSI_RESET);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

inline void Debug(const char *fmt, ...) {
  if (log_level < LogLevel::DEBUG) {
    return;
  }
  va_list args;
  printf("%s[Debug]%s ", ANSI_BOLD_MAGENTA, ANSI_RESET);
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

#endif //INC_8_BIT_CPU_COMMON_LOG_H_
