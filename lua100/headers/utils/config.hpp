#pragma once

// namespace toml { class table; }

namespace lua100::utils::config
{
  auto create_default(void)->toml::table;

  auto load(void)->toml::table;

  constexpr auto default_use_log_console(void) -> bool
  {
#ifdef _DEBUG
    return true;
#else
    return false;
#endif // _DEBUG
  }

  constexpr auto default_log_level(void) -> std::string
  {
#ifdef _DEBUG
    return std::string{ SPDLOG_LEVEL_NAME_DEBUG };
#else
    return std::string{ SPDLOG_LEVEL_NAME_OFF };
#endif // _DEBUG
  }
}