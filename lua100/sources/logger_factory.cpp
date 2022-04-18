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
    const auto name{ _path.filename().replace_extension(".txt")};
    const auto filepath{ path / name };

    std::vector<spdlog::sink_ptr> sinks{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath.string(), true) };

    if (m_use_console) {
      sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always));
    }

    const auto logger{ std::make_shared<spdlog::logger>(_path.filename().stem().string(), sinks.begin(), sinks.end())};
    logger->set_level(m_level);

    return logger;
  }

  logger_factory::logger_factory(const std::string& _path, const std::string& _level, const bool _use_console)
    : path{ _path }
    , m_level{ lvl::from_str(_level) }
    , m_use_console{ _use_console }
  {}
}
