#ifndef LIM_KERNEL
#define LIM_KERNEL

#include "common/common.hpp"
#include "common/module.hpp"

namespace lim::kernel {
    Status Probe(void);
    Status Create(void);
    void   Update(void);
    void   Delete(void);
    bool   Active(void);
};

#endif