#include "nova/logger/logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/pattern_formatter.h>

// ======================= Custom Level Formatter =======================
class custom_level_formatter : public spdlog::custom_flag_formatter {
public:
    void format(const spdlog::details::log_msg& msg, const std::tm&, spdlog::memory_buf_t& dest) override {
        std::string level_str;
        switch (msg.level) {
            case spdlog::level::info:  level_str = " INFO "; break;
            case spdlog::level::warn:  level_str = " WARN "; break;
            case spdlog::level::err:   level_str = " EROR "; break;
            case spdlog::level::debug: level_str = " DEBG "; break;
            default:                   level_str = " UNKN "; break;
        }
        dest.append(level_str.data(), level_str.data() + level_str.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override {
        return spdlog::details::make_unique<custom_level_formatter>();
    }
};

// ======================= Logger Class =======================
namespace Nova {

Logger::Logger(const std::string& name) {
    static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> shared_sink = []() {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sink->set_color(spdlog::level::info,  "\x1b[42m\x1b[30m");
        sink->set_color(spdlog::level::warn,  "\x1b[43m\x1b[30m");
        sink->set_color(spdlog::level::err,   "\x1b[41m\x1b[37m");
        sink->set_color(spdlog::level::debug, "\x1b[46m\x1b[30m");
        return sink;
    }();

    logger_ = std::make_shared<spdlog::logger>(name, shared_sink);
    logger_->set_level(spdlog::level::debug);

    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<custom_level_formatter>('*').set_pattern(
        "\x1b[90m[\x1b[0m\x1b[90m%H:%M:%S\x1b[0m\x1b[90m]\x1b[0m "
        "%^%*%$ "
        "\x1b[90m\x1b[0m\x1b[47m\x1b[30m %n \x1b[0m\x1b[90m\x1b[0m "
        "\x1b[90m[\x1b[0m\x1b[90m%s:%#\x1b[0m\x1b[90m]\x1b[0m "
        "\x1b[97m%v\x1b[0m"
    );
    logger_->set_formatter(std::move(formatter));

    spdlog::register_logger(logger_);
}

Logger::~Logger() {
    spdlog::drop(logger_->name());
}

void Logger::log_with_location(spdlog::level::level_enum level, const std::string& message,
                                const char* file, int line, const char* func) const {
    logger_->log(spdlog::source_loc{file, line, func}, level, message);
}

std::shared_ptr<spdlog::logger> Logger::get() const {
    return logger_;
}

void Logger::shutdown() {
    spdlog::shutdown();
}

} // namespace Nova