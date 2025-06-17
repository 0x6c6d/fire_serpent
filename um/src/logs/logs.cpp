#include "logs.h"
#include <cstdarg>
#include <cstdio>

namespace logs {
    static std::vector<logs::LogEntry> log_messages;
    static std::mutex log_mutex;

    void log(logs::Level level, const char* fmt, ...) {
        char buffer[1024];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        std::lock_guard<std::mutex> lock(log_mutex);
        log_messages.push_back({ buffer, level });
    }

    const std::vector<logs::LogEntry>& get_logs() {
        return log_messages;
    }
}
