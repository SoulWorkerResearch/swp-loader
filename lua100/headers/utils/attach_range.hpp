#pragma once

// local
#include "../plugins.hpp"

// cpp
#include <filesystem>

// deps
#include <swpsdk/game/game_version.hpp>

namespace lua100::utils
{
  class attach_range final
  {
  public:
    auto operator()(const std::filesystem::directory_entry& _entry) const -> void;

    attach_range(plugins::plugins_t& _plugins);

  private:
    plugins::plugins_t& m_plugins;

    const swpsdk::game::version m_game_version;
  };
}