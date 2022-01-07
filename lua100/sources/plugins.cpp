// local
#include "../headers/plugins.hpp"
#include "../headers/utils/game_version.hpp"

// windows
#include <Windows.h>

// local deps
#include <swpsdk/plugin/plugin_emplace.hpp>
#include <swpsdk/plugin/plugin_loader.hpp>

// deps
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

// cpp
#include <system_error>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

lua100::plugins lua100::plugins::instance;

auto lua100::plugins::attach(void) -> void
{
  const auto game_version{ utils::game_version::read() };
  spdlog::info("attach plugins to {} game version", game_version);

  const auto path{ directory() };
  if (not exists(path) && not create_directory(path)) {
    spdlog::critical("plugins directory not found.");
    return;
  }

  const auto files{ fs::recursive_directory_iterator{ path } };
  ranges::for_each(files | views::filter(is_dll), [&](const auto& _entry) {
    win::dll plugin{ _entry };

    const auto name{ _entry.path().stem().generic_string() };
    const auto& logger{ m_loggers.emplace_back(stdout_color_mt<spdlog::async_factory>(name, spdlog::color_mode::always)) };

    if (not plugin) {
      logger->error("{}", std::system_category().message(GetLastError()));
      return;
    }

    using emplace_ptr_t = decltype(swpsdk::plugin::emplace)*;
    const auto emplace{ (emplace_ptr_t)GetProcAddress(static_cast<HMODULE>(plugin), "emplace") };
    if (not emplace) {
      logger->error("haven't emplace function.", _entry);
      return;
    }

    const std::unique_ptr<swpsdk::plugin::info> info{ emplace() };
    if (not info) {
      logger->error("can't get info.", _entry);
      return;
    }

    if (info->game_version != game_version) {
      logger->error("mismatch game version [plugin: {}] != [game: {}]", info->game_version, game_version);
      return;
    }

    // TODO: check loader version

    m_plugins.emplace_back(std::forward<win::dll>(plugin));

    using loader_t = swpsdk::plugin::loader;
    std::thread{ &loader_t::ready, reinterpret_cast<const loader_t*>(info->instance), logger }.detach();

    logger->info("registered", _entry);
  });
}

auto lua100::plugins::is_dll(const fs::directory_entry& _value) -> bool
{
  return _value.path().extension() == ".dll";
}

auto lua100::plugins::directory(void) -> std::filesystem::path
{
  using std::filesystem::current_path;
  return current_path() / "plugins";
}