#ifndef LIM_WINDOW
#define LIM_WINDOW

#include "common/common.hpp"
#include "common/module.hpp"
#include "common/affine.hpp"

namespace lim::window {
    Status  Probe    (void);
    Status  Create   (void);
    void    Update   (void);
    void    Delete   (void);
    void*   Handle   (void);
    i32vec2 GetSize  (void);
    i32vec3 GetDepth (void);
};

#endif