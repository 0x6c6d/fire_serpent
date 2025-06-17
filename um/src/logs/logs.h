#pragma once
#include <vector>
#include <string>
#include <mutex>

namespace logs {
    enum class Level {
        Info,
        Warning,
        Error
    };

    struct LogEntry {
        std::string message;
        Level level;
    };

    void log(Level level, const char* fmt, ...);
    const std::vector<LogEntry>& get_logs();
}
