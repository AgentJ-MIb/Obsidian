#pragma once

#include <stdlib.h>
#define ObsidianVersion "v0.0.1"

#define TRACE_EXECUTION

enum ExitValue {
  OK = 0,
  INVALID_FILE_EXTENSION = 1,
  INVALID_FILE = 2,
  LEXER_ERROR = 3,
  PARSER_ERROR = 4,
  _ERROR = 5,
  MEMORY_ALLOCATION_FAILURE = 6,
  INVALID_ARGUMENT = 7,
  VM_ERROR = 8,
  VM_RETURN = 9,
};

inline void Exit(ExitValue exitValue) { exit(ExitValue(exitValue)); }
