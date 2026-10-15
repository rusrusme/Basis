//
// Copyright (c) 2026 rusrusme 
// All rights reserved.
//

#pragma once

#include <format>
#include <mutex>
#include <string>
#include <vector>

namespace basis {
    enum class LogLevel { Trace, Info, Warn, Error, Critical };

    struct LogEntry {
        std::string message;
        LogLevel level;
        std::string timestamp;
    };

    class Log {
    public:
        static void init();

        template<typename... Args>
        static void log(LogLevel level, const char* category, std::format_string<Args...> fmt, Args&&... args);

        static const std::vector<LogEntry>& get_entries();
        static void clear();

    private:
        static void add_entry(LogLevel level, const char* category, const std::string& message);

        static inline std::vector<LogEntry> entries_;
        static inline std::mutex mutex_;
    };

    template<typename... Args>
    void Log::log(const LogLevel level, const char* category, std::format_string<Args...> fmt, Args&&... args) {
        const std::string formatted_msg = std::format(fmt, std::forward<Args>(args)...);
        add_entry(level, category, formatted_msg);
    }

    namespace util {
        // Copy
        std::string to_lowercase(std::string str);
        std::string to_uppercase(std::string str);
        // In-place
        void to_lowercase_inplace(std::string& str);
        void to_uppercase_inplace(std::string& str);
    }
}

// -----------------------------------------------------------------------------------------------
// Core Logging Macros
#define CORE_TRACE(...)    ::basis::Log::log(::basis::LogLevel::Trace,    "CORE ", __VA_ARGS__)
#define CORE_INFO(...)     ::basis::Log::log(::basis::LogLevel::Info,     "CORE ", __VA_ARGS__)
#define CORE_WARN(...)     ::basis::Log::log(::basis::LogLevel::Warn,     "CORE [WARN]", __VA_ARGS__)
#define CORE_ERROR(...)    ::basis::Log::log(::basis::LogLevel::Error,    "CORE [ERROR]", __VA_ARGS__)
#define CORE_CRITICAL(...) ::basis::Log::log(::basis::LogLevel::Critical, "CORE [CRITICAL]", __VA_ARGS__)

// -----------------------------------------------------------------------------------------------
// Client Application Logging Macros
#define CLIENT_TRACE(...)    ::basis::Log::log(::basis::LogLevel::Trace,    "APP", __VA_ARGS__)
#define CLIENT_INFO(...)     ::basis::Log::log(::basis::LogLevel::Info,     "APP ", __VA_ARGS__)
#define CLIENT_WARN(...)     ::basis::Log::log(::basis::LogLevel::Warn,     "APP [WARN]", __VA_ARGS__)
#define CLIENT_ERROR(...)    ::basis::Log::log(::basis::LogLevel::Error,    "APP [ERROR]", __VA_ARGS__)
#define CLIENT_CRITICAL(...) ::basis::Log::log(::basis::LogLevel::Critical, "APP [CRITICAL]", __VA_ARGS__)
