// local
#include "../headers/plugins.hpp"

// windows
#include <Windows.h>

// local deps
#include <sdk/plugin/plugin_emplace.hpp>
#include <sdk/plugin/plugin_loader.hpp>

// deps
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

// cpp
#include <system_error>
#include <filesystem>

using namespace std;
using namespace std::filesystem;
using namespace std::ranges::views;
using namespace spdlog;

lua100::plugins lua100::plugins::instance;

auto lua100::plugins::attach(void) -> void
{
  const auto path{ directory() };
  if (not exists(path) && not create_directory(path)) {
    spdlog::critical("plugins directory not found.");
    return;
  }

  const auto files{ recursive_directory_iterator{ path } };
  for (auto& entry : files | filter(is_dll))
  {
    win::dll plugin{ entry };

    const auto dll_name{ entry.path().stem().generic_string() };
    const auto& logger{ m_loggers.emplace_back(stdout_color_mt<async_factory>(dll_name, color_mode::always)) };

    if (not plugin) {
      logger->error("{}", system_category().message(GetLastError()));
      continue;
    }

    using emplace_ptr_t = decltype(sdk::plugin::emplace)*;
    const auto emplace{ (emplace_ptr_t)GetProcAddress(static_cast<HMODULE>(plugin), "emplace") };
    if (not emplace) {
      logger->error("haven't emplace function.", entry);
      continue;
    }

    const unique_ptr<sdk::plugin::info> info{ emplace() };
    if (not info) {
      logger->error("can't get info.", entry);
      continue;
    }

    // TODO: check game version

    m_plugins.emplace_back(forward<win::dll>(plugin));

    using loader_t = sdk::plugin::loader;
    thread{ &loader_t::ready, reinterpret_cast<const loader_t*>(info->instance), logger }.detach();

    logger->info("registered", entry);
  }
}

auto lua100::plugins::is_dll(const directory_entry& _value) -> bool
{
  return _value.path().extension() == ".dll";
}

auto lua100::plugins::directory(void) -> std::filesystem::path
{
  using std::filesystem::current_path;
  return current_path() / "plugins";
}

template<typename TStream>
TStream& operator<<(TStream& _os, const directory_entry& _value)
{
  return _os << _value.path().c_str();
}