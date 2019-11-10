#ifndef LIBS_LOGGER_LOGGER_H_
#define LIBS_LOGGER_LOGGER_H_

#include <cstdint>

/*
 * Logging levels.
 * User should set LOG_LEVEL macro to one of those levels.
 */
#define LOG_LEVEL_NONE (-1)
#define LOG_LEVEL_ALWAYS 0
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_INFO 4
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_ALL 0xFF

/*
 * Every macro prints the log information with additional level message and
 * newline.
 * User should use of those macros:
 *
 * LOG_ALWAYS
 * LOG_FATAL
 * LOG_ERROR
 * LOG_WARNING
 * LOG_INFO
 * LOG_DEBUG
 */

#if LOG_LEVEL < LOG_LEVEL_ALWAYS
#define LOG_ALWAYS(format, ...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_ALWAYS(format, ...) \
    logger::print_log(LOG_LEVEL_ALWAYS, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif

#if LOG_LEVEL < LOG_LEVEL_FATAL
#define LOG_FATAL(format, ...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_FATAL(format, ...) \
    logger::print_log(LOG_LEVEL_FATAL, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif

#if LOG_LEVEL < LOG_LEVEL_ERROR
#define LOG_ERROR(...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_ERROR(format, ...) \
    logger::print_log(LOG_LEVEL_ERROR, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif

#if LOG_LEVEL < LOG_LEVEL_WARNING
#define LOG_WARNING(...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_WARNING(format, ...) \
    logger::print_log(LOG_LEVEL_WARNING, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif

#if LOG_LEVEL < LOG_LEVEL_INFO
#define LOG_INFO(...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_INFO(format, ...) \
    logger::print_log(LOG_LEVEL_INFO, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif

#if LOG_LEVEL < LOG_LEVEL_DEBUG
#define LOG_DEBUG(...) logger::null_logger(__VA_ARGS__)
#else
#define LOG_DEBUG(format, ...) \
    logger::print_log(LOG_LEVEL_DEBUG, __FILENAME__, __LINE__, format, ##__VA_ARGS__)
#endif


namespace logger {
    void print_log(std::uint8_t, const char*, int, const char*, ...);
    static constexpr void null_logger(...) {
    }
}  // namespace logger

#endif  // LIBS_LOGGER_LOGGER_H_