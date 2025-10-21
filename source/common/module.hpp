#ifndef LIM_COMMON_MODULE
#define LIM_COMMON_MODULE

#include "common/common.hpp"

namespace lim {
    enum class Status : u64 {
        EMPTY  = 0,
        ACTIVE = 1,
        FAILED = 2,
    };
};

#endif