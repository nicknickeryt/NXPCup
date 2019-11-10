#include "logger.h"
#include <cstdio>
#include <cstdarg>

namespace logger {

constexpr static char _level_always[]  = "[ALWAYS] ";
constexpr static char _level_fatal[]   = "[FATAL]  ";
constexpr static char _level_error[]   = "[ERROR]  ";
constexpr static char _level_warning[] = "[WARN]   ";
constexpr static char _level_info[]    = "[INFO]   ";
constexpr static char _level_debug[]   = "[DEBUG]  ";
constexpr static char _newline[]       = "\n";

constexpr static const char* _level_table[] = {_level_always,
                                               _level_fatal,
                                               _level_error,
                                               _level_warning,
                                               _level_info,
                                               _level_debug};

void print_log(uint8_t level, const char* filename, int line, const char* format, ...) {
    std::va_list args;
    va_start(args, format);

    printf("%s%s:%d ", _level_table[level], filename, line);
    vfprintf(stdout, format, args);
    printf(_newline);

    va_end(args);
}

}  // namespace logger