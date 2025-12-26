#pragma once
#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace Nova {

class Logger {
public:
    Logger(const std::string& name);
    ~Logger();

    void log_with_location(spdlog::level::level_enum level, const std::string& message,
                          const char* file, int line, const char* func) const;
    
    std::shared_ptr<spdlog::logger> get() const;
    
    static void shutdown();

private:
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace Nova

// Macros to capture caller's file and line
#define NOVA_LOG(logger, level, msg) \
    (logger).log_with_location(level, msg, __FILE__, __LINE__, SPDLOG_FUNCTION)

#define NOVA_LOW_LOG_INFO(logger, msg)  NOVA_LOG(logger, spdlog::level::info, msg)
#define NOVA_LOW_LOG_WARN(logger, msg)  NOVA_LOG(logger, spdlog::level::warn, msg)
#define NOVA_LOW_LOG_ERR(logger, msg) NOVA_LOG(logger, spdlog::level::err, msg)
#define NOVA_LOW_LOG_DEBG(logger, msg) NOVA_LOG(logger, spdlog::level::debug, msg)