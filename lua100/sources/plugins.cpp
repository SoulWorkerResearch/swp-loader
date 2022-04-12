// local
#include "../headers/plugins.hpp"
#include "../headers/utils/plugin_loader.hpp"

// local deps
#include <swpsdk/defines.hpp>
#include <swpsdk/utils/spdlog_formatter.hpp>

// deps
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

lua100::plugins lua100::plugins::instance;

auto lua100::plugins::attach(const logger_factory_t& _logger) -> void
{
  spdlog::info("sdk v{}", swpsdk::current_version);

  const auto path{ directory() };
  if (not exists(path) && not create_directory(path)) {
    spdlog::critical("plugins directory not found.");
    return;
  }

  utils::plugin_loader loader{ _logger };
  for (const auto& entry : fs::recursive_directory_iterator{ path } | views::filter(is_dll)) {
    auto plugin{ loader(entry) };
    if (not plugin) continue;

    m_plugins.emplace_back(std::forward<decltype(plugin)>(plugin));
  }
}

auto lua100::plugins::is_dll(const fs::directory_entry& _value) -> bool
{
  return _value.is_regular_file() && _value.path().extension() == ".dll";
}

auto lua100::plugins::directory(void) -> std::filesystem::path
{
  return fs::current_path() / "plugins";
}