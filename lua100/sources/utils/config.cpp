// local
#include "../../headers/utils/config.hpp"

// deps
#include <spdlog/spdlog.h>

// cpp
#include <filesystem>

namespace level = spdlog::level;
namespace fs = std::filesystem;

namespace utils::config
{
  auto create_default(void) -> toml::table
  {
    return toml::table{
        { "logger", toml::table{ { "use_console", false }, { "level", std::string{ SPDLOG_LEVEL_NAME_OFF }}}
      }
    };
  }

  auto load(void) -> toml::table
  {
    const fs::path path{ "configs/loader.toml" };
    
    if (!fs::exists(path)) {
      const auto config{ create_default() };

      fs::create_directories(path.parent_path());
      
      std::ofstream{ path } << config;

      return config;
    }

    return toml::parse_file(path.c_str());
  }
}