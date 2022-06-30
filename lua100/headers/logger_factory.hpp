#pragma once

namespace lua100
{
  class logger_factory final
  {
  public:

#pragma region Types

    using type_value = std::shared_ptr<logger_factory>;

#pragma endregion Types

#pragma region Members

    const std::filesystem::path path;
    const spdlog::level::level_enum level;
    const bool use_console;

#pragma endregion Members

#pragma region Methods

    auto create(const std::filesystem::path& _path) const->std::shared_ptr<spdlog::logger>;

#pragma endregion Methods

#pragma region Constructors

    logger_factory(const std::filesystem::path& _path, const std::string& _level, const bool _use_console);

#pragma endregion Constructors

  };
}
