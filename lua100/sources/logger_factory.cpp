// local
#include "../headers/logger_factory.hpp"

// deps
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// cpp
#include <vector>

namespace fs = std::filesystem;
namespace lvl = spdlog::level;

namespace lua100
{
  auto logger_factory::create(const std::filesystem::path& _path) const -> std::shared_ptr<spdlog::logger>
  {
    const auto name{ _path.filename().stem() };

    namespace level = spdlog::level;
    namespace fs = std::filesystem;

    const auto filename{ std::format("{:%FT%H-%M-%S}", std::chrono::system_clock::now()) };
    const auto path{ fs::path{ "logs" } / name / m_time };

    std::vector<spdlog::sink_ptr> sinks{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string()) };

    if (m_use_console) {
      sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always));
    }

    const auto logger{ std::make_shared<spdlog::logger>(name.string(), sinks.begin(), sinks.end()) };
    logger->set_level(m_level);

    return logger;
  }

  logger_factory::logger_factory(const std::string& _level, const bool _use_console)
    : m_time{ fs::path{ std::format("{:%FT%H-%M-%S}", std::chrono::system_clock::now())}.replace_extension("txt") }
    , m_level{ lvl::from_str(_level) }
    , m_use_console{ _use_console }
  {}
}
