// local
#include "../../headers/utils/attach_range.hpp"
#include "../../headers/utils/game_version.hpp"
#include "../../headers/utils/spdlog_formatter.hpp"
#include "../../headers/plugins.hpp"

// windows
#include <Windows.h>

// local deps
#include <swpsdk/plugin/plugin_attach.hpp>
#include <swpsdk/plugin/plugin_loader.hpp>

// deps
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// cpp
#include <system_error>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;

auto lua100::utils::attach_range::operator()(const fs::directory_entry& _entry) const -> void
{
  win::dll plugin{ _entry };

  const auto name{ _entry.path().stem().generic_string() };
  const auto logger{ stdout_color_mt<spdlog::async_factory>(name, spdlog::color_mode::always) };

  if (not plugin) {
    logger->error("{}", std::system_category().message(GetLastError()));
    return;
  }

  using attach_ptr_t = decltype(swpsdk::plugin::attach)*;
  const auto attach{ (attach_ptr_t)GetProcAddress(static_cast<HMODULE>(plugin), "attach") };
  if (not attach) {
    logger->error("haven't attach function.", _entry);
    return;
  }

  const std::unique_ptr<swpsdk::plugin::info> info{ attach() };
  if (not info) {
    logger->error("can't get info.", _entry);
    return;
  }

  if (info->game_version != m_game_version) {
    logger->error("mismatch game version [plugin: {}] != [game: {}]", info->game_version, m_game_version);
    return;
  }

  // TODO: check loader version

  m_plugins.emplace_back(std::forward<win::dll>(plugin));

  using loader_t = swpsdk::plugin::loader;
  std::thread{ &loader_t::ready, reinterpret_cast<const loader_t*>(info->instance), logger }.detach();

  logger->info("attached v{}", info->plugin_version);
}

lua100::utils::attach_range::attach_range(plugins::plugins_t& _plugins) :
  m_plugins{ _plugins },
  m_game_version{ utils::game_version::read() }
{
  spdlog::info("game v{}", m_game_version);
}
