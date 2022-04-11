#pragma once

// local
#include "../plugins.hpp"

// cpp
#include <filesystem>

// deps
#include <swpsdk/version.hpp>

namespace lua100::utils
{
  class plugin_loader final
  {
  public:
    auto operator()(const std::filesystem::directory_entry& _entry) const ->win::dll;

    plugin_loader(void);

  private:
    const swpsdk::version m_game_version;
  };
}