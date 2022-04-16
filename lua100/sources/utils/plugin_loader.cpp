// local
#include "../../headers/utils/plugin_loader.hpp"
#include "../../headers/utils/game_version.hpp"
#include "../../headers/plugins.hpp"

// windows
#include <Windows.h>

// local deps
#include <swpsdk/utils/spdlog_formatter.hpp>
#include <swpsdk/plugin/attach.hpp>
#include <swpsdk/plugin/loader.hpp>

// deps
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// cpp
#include <system_error>
#include <filesystem>
#include <ranges>
#include <future>

namespace fs = std::filesystem;

auto create_logger(const std::string& _name)
{
  return spdlog::stdout_color_mt<spdlog::async_factory>(_name, spdlog::color_mode::always);
}

auto lua100::utils::plugin_loader::operator()(const fs::directory_entry& _entry) const->std::unique_ptr<plugin_info>
{
  win::dll dll{ _entry };

  auto logger{ m_logger_factory->create(_entry.path()) };

  if (not dll) {
    logger->error(std::system_category().message(GetLastError()));
    return nullptr;
  }

  using attach_ptr_t = decltype(swpsdk::plugin::attach)*;
  const auto address{ GetProcAddress(static_cast<HMODULE>(dll), "attach") };
  const auto attach{ reinterpret_cast<attach_ptr_t>(address) };
  if (not attach) {
    logger->error("haven't attach function.", _entry);
    return nullptr;
  }

  const std::unique_ptr<swpsdk::plugin::info> info{ attach() };
  if (not info) {
    logger->error("can't get info.", _entry);
    return nullptr;
  }

  if (info->game_version != m_game_version) {
    logger->warn("mismatch game version [plugin: {}] != [game: {}]", info->game_version, m_game_version);
  }

  if (info->sdk_version != swpsdk::current_version) {
    logger->warn("mismatch sdk version [plugin: {}] != [loader: {}]", info->sdk_version, swpsdk::current_version);
  }

  using loader_t = swpsdk::plugin::loader;

  auto p{ reinterpret_cast<const loader_t*>(info->instance) };
  std::invoke(&loader_t::attach, p, logger);

  logger->info("attached v{}", info->plugin_version);
  return std::make_unique<plugin_info>(std::forward<decltype(dll)>(dll), std::forward<decltype(logger)>(logger));
}

lua100::utils::plugin_loader::plugin_loader(const logger_factory_t& _logger_factory)
  : m_game_version{ utils::game_version::read() }
  , m_logger_factory{ _logger_factory }
{
  spdlog::info("game v{}", m_game_version);
}
