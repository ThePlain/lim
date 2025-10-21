#ifndef LIM_LOGGER
#define LIM_LOGGER

#include "common/common.hpp"
#include "common/module.hpp"

namespace lim::logger {
    enum Level {
        eLevelDebug,
        eLevelInfo,
        eLevelWarning,
        eLevelError,
        eLevelPanic,
    };

    Status Probe(void);
    Status Create(void);
    void   Update(void);
    void   Delete(void);
    void   Record(Level lvl, const char* module, const char* fmt, ...);
    void   Flush(void);
};

#ifndef LOGGER
#define LOGGER "Unknown"
#endif

#define LOG_DEBUG(fmt, ...)   lim::logger::Print(lim::logger::eLevelDebug,   LOGGER, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)    lim::logger::Print(lim::logger::eLevelInfo,    LOGGER, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) lim::logger::Print(lim::logger::eLevelWarning, LOGGER, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)   lim::logger::Print(lim::logger::eLevelError,   LOGGER, fmt, ##__VA_ARGS__)
#define LOG_PANIC(fmt, ...)   lim::logger::Print(lim::logger::eLevelPanic,   LOGGER, fmt, ##__VA_ARGS__)

#endif