// local
#include "utils/plugin_loader.hpp"
#include "utils/game_version.hpp"
#include "plugins.hpp"

namespace fs = std::filesystem;

auto lua100::utils::plugin_loader::operator()(const fs::directory_entry& _entry) const->std::unique_ptr<plugin_info>
{
  auto logger{ m_logger_factory->create(_entry.path()) };

  win::dll dll{ _entry };
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

lua100::utils::plugin_loader::plugin_loader(const logger_factory::type_value& _logger_factory)
  : m_game_version{ utils::game_version::read() }
  , m_logger_factory{ _logger_factory }
{
  spdlog::info("game v{}", m_game_version);
}
