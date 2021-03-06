// local
#include "plugins.hpp"
#include "utils/plugin_loader.hpp"

#include <fmt/format.h>

namespace fs = std::filesystem;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

auto is_dll(const fs::directory_entry& _value) -> bool
{
  return _value.path().extension() == ".dll";
}

auto directory(void) -> std::filesystem::path
{
  return fs::current_path() / "plugins";
}


auto lua100::plugins::attach(const logger_factory::type_value& _logger) -> void
{
  spdlog::info("sdk v{}", swpsdk::current_version);

  const auto path{ directory() };
  if (not exists(path) && not create_directory(path)) {
    spdlog::critical("plugins directory not found.");
    return;
  }

  utils::plugin_loader loader{ _logger };
  for (const auto& entry : fs::recursive_directory_iterator{ path } | views::filter(is_dll)) {
    const auto plugin{ loader(entry) };
    if (not plugin) continue;

    m_plugins.emplace_back(std::forward<decltype(plugin)::element_type>(*plugin));
  }
}
