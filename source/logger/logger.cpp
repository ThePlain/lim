#include "common/common.hpp"
#include "common/module.hpp"
#include "common/string.hpp"
#include "logger/logger.hpp"

#include <windows.h>
#include <fstream>
#include <chrono>

namespace lim::logger {
    using File = std::ofstream;

    static struct {
        String mPath      { "./lim.log"               };
        String mTime      { "YYYY-MM-DD HH:MM:SS"     };
        String mStamp     { "YYYY-MM-DD HH:MM:SS.UUU" };
        String mBuffer    { 2048                      };
        File   mHandler   {                           };
        bool   mHasStream { false                     };
        bool   mHasFile   { false                     };
        Status mStatus    { Status::EMPTY             };
    } self;

    const char* _LevelName(Level level) {
        switch (level) {
            case eLevelDebug:   return "DEBUG";
            case eLevelInfo:    return "INFO";
            case eLevelWarning: return "WARNING";
            case eLevelError:   return "ERROR";
            case eLevelPanic:   return "PANIC";
            default:            return "UNKNOWN";
        }
    };

    const char* _LevelColor(Level level) {
        switch (level) {
            case eLevelDebug:   return "\033[90m";
            case eLevelInfo:    return "\033[0m";
            case eLevelWarning: return "\033[93m";
            case eLevelError:   return "\033[91m";
            case eLevelPanic:   return "\033[95m";
            default:            return "\033[0m";
        };
    };

    bool _InitFile() {
        self.mHandler.open(self.mPath, std::ofstream::out | std::ofstream::trunc);
        if (!self.mHandler.is_open())
            return false;
        return true;
    };

    bool _InitStream() {
        if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
            DWORD error = GetLastError();
            if (error == ERROR_ACCESS_DENIED) {
            }
            else if (error == ERROR_INVALID_HANDLE) {
                if (!AllocConsole()) {
                    return false;
                }
            }
            else {
                return false;
            }
        }

        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

        // Enable ANSI BEFORE freopen
        HANDLE hOut = CreateFileW(
            L"CONOUT$",
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            GetConsoleMode(hOut, &dwMode);
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
            CloseHandle(hOut);
        }

        // Now reopen streams
        FILE* dummy;
        freopen_s(&dummy, "CONOUT$", "w", stdout);
        freopen_s(&dummy, "CONIN$", "r", stdin);
        freopen_s(&dummy, "CONOUT$", "w", stderr);

        return true;
    };

    Status Probe(void) {
        return self.mStatus;
    };

    Status Create(void) {
        if (self.mStatus == Status::ACTIVE) {
            return self.mStatus;
        }

        self.mHasFile   = _InitFile();
        self.mHasStream = _InitStream();

        self.mStatus = self.mHasFile || self.mHasStream ? Status::ACTIVE : Status::FAILED;

        return self.mStatus;
    };

    void Update(void) {
        if (self.mStatus != Status::ACTIVE) {
            return;
        }

        Flush();
    };

    void Delete(void) {
        if (self.mStatus == Status::EMPTY) {
            return;
        }

        if (self.mHandler.is_open()) {
            self.mHandler.flush();
            self.mHandler.close();
        }

        if (self.mHasStream) {
            FreeConsole();
        }

        self.mHasFile   = false;
        self.mHasStream = false;
        self.mStatus    = Status::EMPTY;
    };

    void Record(Level lvl, const char* module, const char* fmt, ...) {
        if (self.mStatus != Status::ACTIVE) {
            return;
        }

        va_list args;
        va_start(args, fmt);
        int32_t size = std::vsnprintf(self.mBuffer, self.mBuffer.capacity(), fmt, args);
        va_end(args);

        if (size <= 0)
            return;

        std::tm tm_buf;
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        localtime_s(&tm_buf, &time_t);

        std::strftime(self.mTime, self.mTime.capacity(), "%Y-%m-%d %H:%M:%S", &tm_buf);
        std::snprintf(self.mStamp, self.mStamp.capacity(), "%s.%03d", self.mTime, static_cast<int>(ms.count()));

        const char* lname = _LevelName(lvl);

        if (self.mHasFile) {
            self.mHandler << lname << " " << self.mStamp << " " << module << " - " << self.mBuffer << std::endl;
            self.mHandler.flush();
        }

        if (self.mHasStream) {
            const char* clr_s = _LevelColor(lvl);
            const char* clr_e = "\033[0m";
            std::printf("%s%-8s %s %s - %s%s\n", clr_s, lname, self.mStamp, module, self.mBuffer, clr_e);
        }
    };

    void Flush(void) {
        if (self.mHandler && self.mHandler.is_open()) {
            self.mHandler.flush();
        }
    };
};