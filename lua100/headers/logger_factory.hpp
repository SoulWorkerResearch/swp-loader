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

#pragma region Members

    const std::filesystem::path path;

#pragma endregion Members

#pragma region Methods

    auto create(const std::filesystem::path& _path) const->std::shared_ptr<spdlog::logger>;

#pragma endregion Methods

#pragma region Constructors

    logger_factory(const std::string& _path, const std::string& _level, const bool _use_console);

#pragma endregion Constructors

  private:

#pragma region Members

    const spdlog::level::level_enum m_level;
    const bool m_use_console;

#pragma endregion Members

  };

  using logger_factory_t = std::shared_ptr<logger_factory>;
}