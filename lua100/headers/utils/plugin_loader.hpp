#pragma once

// local
#include "../plugins.hpp"
#include "../logger_factory.hpp"

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

    plugin_loader(const logger_factory_t& _logger_factory);

  private:
    const swpsdk::version m_game_version;
    const logger_factory_t m_logger_factory;
  };
}