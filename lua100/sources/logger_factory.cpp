// local
#include "logger_factory.hpp"

namespace fs = std::filesystem;
namespace lvl = spdlog::level;

namespace lua100
{
  auto logger_factory::create(const fs::path& _path) const -> std::shared_ptr<spdlog::logger>
  {
    const auto name{ _path.filename().replace_extension(".txt")};
    const auto filepath{ path / name };

    std::vector<spdlog::sink_ptr> sinks{ std::make_shared<spdlog::sinks::basic_file_sink_mt>(filepath.string(), true) };

    if (use_console) {
      sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>(spdlog::color_mode::always));
    }

    const auto logger{ std::make_shared<spdlog::logger>(_path.filename().stem().string(), sinks.begin(), sinks.end())};
    logger->set_level(level);

    return logger;
  }

  logger_factory::logger_factory(const fs::path& _path, const std::string& _level, const bool _use_console)
    : path{ _path }
    , level{ lvl::from_str(_level) }
    , use_console{ _use_console }
  {}
}
