#ifndef LIM_COMMON_STRING
#define LIM_COMMON_STRING

#include "common/common.hpp"

namespace lim {
    union String {
    private:
        struct { size_t used; char  buffer[24];          } sso;
        struct { size_t used; char* buffer; size_t size; } raw;
    public:
       ~String();
        String();
        String(String&& other);
        String(const char* other);
        String(const String& other);
    public:
        String& operator = (String&& other);
        String& operator = (const char* other);
        String& operator = (const String& other);
    public:
        size_t      length  (void) const noexcept;
        size_t      capacity(void) const noexcept;
        const char* c_str   (void) const noexcept;
        bool        is_sso  (void) const noexcept;
        bool        is_heap (void) const noexcept;
    };
};

#endif