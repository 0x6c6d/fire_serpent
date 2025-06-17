#include "logs.h"
#include <cstdarg>
#include <cstdio>

namespace logs {
    // list of in memory log msgs
    // -static: makes it private
    static std::vector<logs::LogEntry> log_msgs;
    // synchronize access to log_msgs across multiple threads
    static std::mutex log_mutex;

    void log(logs::Level level, const char* fmt, ...) {
        char buffer[1024];

        va_list args;
        va_start(args, fmt);
        // only writes sizeof(buffer) - 1 chars to the buffer -> buffer is null terminated
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        std::lock_guard<std::mutex> lock(log_mutex);
        log_msgs.push_back({ buffer, level });
    }

    const std::vector<logs::LogEntry>& get_logs() {
        return log_msgs;
    }
}
