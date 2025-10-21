#include "common/common.hpp"
#include "common/string.hpp"

#include <new>

namespace lim {
    bool String::is_sso() const noexcept {
        return sso.used < 24;
    }

    bool String::is_heap() const noexcept {
        return sso.used >= 24;
    }

    size_t String::length() const noexcept {
        return sso.used;
    }

    size_t String::capacity() const noexcept {
        return is_sso() ? 23 : raw.size - 1;
    }

    const char* String::c_str() const noexcept {
        return is_sso() ? sso.buffer : raw.buffer;
    }

    String::String() {
        sso.used = 0;
        sso.buffer[0] = '\0';
    };

    String::String(const char* other) {
        if (!other) {
            sso.used      = 0;
            sso.buffer[0] = '\0';
            return;
        }

        u64 len = strlen(other);
        if (len < 24) {
            sso.used = len;
            memcpy(sso.buffer, other, len + 1);
        }
        else {
            raw.used   = len;
            raw.size   = len + 1;
            raw.buffer = new char[raw.size];
            memcpy(raw.buffer, other, len + 1);
        }
    };

    String::String(const String& other) {
        if (other.is_sso()) {
            sso = other.sso;
        }
        else {
            raw.used   = other.raw.used;
            raw.size   = other.raw.size;
            raw.buffer = new char[raw.size];
            memcpy(raw.buffer, other.raw.buffer, raw.used + 1);
        }
    };

    String::String(String&& other) {
        memcpy(this, &other, sizeof(String));

        other.sso.used = 0;
        other.sso.buffer[0] = '\0';
    };

    String::~String() {
        if (is_heap()) {
            delete[] raw.buffer;
        }
    };

    String& String::operator=(const String& other) {
        if (this != &other) {
            this->~String();
            new (this) String(other);
        }
        return *this;
    };

    String& String::operator=(String&& other) {
        if (this != &other) {
            this->~String();
            new (this) String(std::move(other));
        }
        return *this;
    };

    String& String::operator=(const char* other) {
        this->~String();
        new (this) String(other);
        return *this;
    };
};