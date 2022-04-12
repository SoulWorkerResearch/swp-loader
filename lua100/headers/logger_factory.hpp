#pragma once

// deps
#include <spdlog/spdlog.h>

// cpp
#include <string_view>
#include <filesystem>
#include <memory>

namespace lua100
{
  class logger_factory final
  {
  public:
    auto create(const std::filesystem::path& _path) const->std::shared_ptr<spdlog::logger>;

    logger_factory(const std::string& _level, const bool _use_console);

  private:
    const std::filesystem::path m_time;
    const spdlog::level::level_enum m_level;
    const bool m_use_console;
  };
  
  using logger_factory_t = std::shared_ptr<logger_factory>;
}