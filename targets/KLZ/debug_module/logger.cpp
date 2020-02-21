//
// Created by kamil on 04.01.2020.
//

#include <inttypes.h>
#include <stdarg.h>
#include <filesystem>
#include <stdio.h>
#include <string.h>

namespace fs = std::filesystem;

#include "printf.h"

#include "logger.h"
#include "NXP_uart.hpp"
#include "NXP_Kitty.hpp"

static const char* const abbreviations[] = {"EMG", "ALT", "CRT ", "ERR", "WRN", "NOT", "INF", "DBG"};

static void writeFunctionWrapper(const char c, void* const context);

// -----------------------------------------------------------------------------
//  HAL_LOG_Log
// -----------------------------------------------------------------------------
void log_write(const uint8_t channel, const uint8_t level, const char* const channelName, const char* const filename, const int line,
               const char* format, ...) {
    (void) channel;
    std::string str = filename;
    size_t shift = str.find_last_of("\\");
    // Checking if channel number is in bounds was done in preprocessing stage
    fctprintf(writeFunctionWrapper, NULL, "%s: %4d %s(%s): ", filename+shift+1, line, channelName,
              abbreviations[level]);
    // Write the very log message
    va_list args;
    va_start(args, format);
    vfctprintf(writeFunctionWrapper, NULL, format, args);
    va_end(args);
    fctprintf(writeFunctionWrapper, NULL, "\n\r");

} /* HAL_LOG_Log */

static void writeFunctionWrapper(const char c, void* const context) {
    (void) context;
    if (c != '\0') {
        Kitty::kitty().uartCommunication.write(c);
    }
}