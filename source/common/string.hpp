#ifndef LIM_COMMON_STRING
#define LIM_COMMON_STRING

#include "common/common.hpp"

namespace lim {
    union String {
    private:
        struct { size_t size; char  buffer[24]; } sso;
        struct { size_t size; char* buffer;     } raw;
    public:
       ~String();
        String();
        String(size_t size);
        String(String&& other);
        String(const char* other);
        String(const String& other);
    public:
        String& operator = (String&& other);
        String& operator = (const char* other);
        String& operator = (const String& other);
    public:
        operator const char* () const noexcept;
        operator char*       ()       noexcept;
    public:
        size_t      length  (void) const noexcept;
        size_t      capacity(void) const noexcept;
        const char* c_str   (void) const noexcept;
        bool        is_sso  (void) const noexcept;
        bool        is_heap (void) const noexcept;
        char*       buffer  (void)       noexcept;
    };
};

#endif