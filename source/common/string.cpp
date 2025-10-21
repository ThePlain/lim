#include "common/common.hpp"
#include "common/string.hpp"

#include <new>

namespace lim {
    String::String() {
        sso.size = 0;
        sso.buffer[0] = '\0';
    };

    String::String(size_t size) {
        if (size < 24) {
            sso.size      = 0;
            sso.buffer[0] = '\0';
        }
        else {
            raw.size      = size;
            raw.buffer    = new char[raw.size];
            raw.buffer[0] = '\0';
        }
    };

    String::String(const char* other) {
        if (!other) {
            sso.size      = 0;
            sso.buffer[0] = '\0';
            return;
        }

        u64 len = strlen(other);
        if (len < 24) {
            sso.size = len;
            memcpy(sso.buffer, other, len + 1);
        }
        else {
            raw.size   = len;
            raw.buffer = new char[raw.size + 1];
            memcpy(raw.buffer, other, len + 1);
        }
    };

    String::String(const String& other) {
        if (other.is_sso()) {
            sso = other.sso;
        }
        else {
            raw.size   = other.raw.size;
            raw.buffer = new char[raw.size + 1];
            memcpy(raw.buffer, other.raw.buffer, raw.size + 1);
        }
    };

    String::String(String&& other) {
        memcpy(this, &other, sizeof(String));

        other.sso.size = 0;
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

    String::operator const char* () const noexcept {
        return c_str();
    };

    String::operator char* () noexcept {
        return buffer();
    };

    bool String::is_sso() const noexcept {
        return sso.size < 24;
    }

    bool String::is_heap() const noexcept {
        return sso.size >= 24;
    }

    size_t String::length() const noexcept {
        return sso.size;
    }

    size_t String::capacity() const noexcept {
        return is_sso() ? 23 : raw.size;
    }

    const char* String::c_str() const noexcept {
        return is_sso() ? sso.buffer : raw.buffer;
    }

    char* String::buffer() noexcept {
        return is_sso() ? sso.buffer : raw.buffer;
    };
};