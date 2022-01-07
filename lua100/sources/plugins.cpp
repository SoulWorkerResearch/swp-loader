// local
#include "../headers/plugins.hpp"
#include "../headers/utils/attach_range.hpp"
#include "../headers/utils/spdlog_formatter.hpp"

// local deps
#include <swpsdk/sdk_version.hpp>

// deps
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

lua100::plugins lua100::plugins::instance;

auto lua100::plugins::attach(void) -> void
{
  spdlog::info("sdk v{}", swpsdk::version);

  utils::attach_range attacher{ m_plugins };

  const auto path{ directory() };
  if (not exists(path) && not create_directory(path)) {
    spdlog::critical("plugins directory not found.");
    return;
  }

  const auto files{ fs::recursive_directory_iterator{ path } };
  ranges::for_each(files | views::filter(is_dll), attacher);
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