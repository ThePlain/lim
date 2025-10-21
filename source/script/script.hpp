#ifndef LIM_SCRIPT
#define LIM_SCRIPT

#include "common/common.hpp"
#include "common/module.hpp"

namespace lim::script {
    struct Script;
    struct Export;

    Status Probe(void);
    Status Create(void);
    void   Update(void);
    void   Delete(void);
};

#endif