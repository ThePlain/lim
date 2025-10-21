#ifndef LIM_RENDER
#define LIM_RENDER

#include "common/common.hpp"
#include "common/module.hpp"

namespace lim::render {
    Status Probe(void);
    Status Create(void);
    void   Update(void);
    void   Delete(void);
};

#endif