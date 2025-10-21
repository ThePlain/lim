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

#endif