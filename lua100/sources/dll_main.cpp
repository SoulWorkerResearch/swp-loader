// windows
#include <Windows.h>
#include <Psapi.h>

// cpp
#include <filesystem>
#include <iostream>
#include <thread>
#include <cstdlib>

// deps
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <toml++/toml.h>

// local
#include "../headers/plugins.hpp"
#include "../headers/utils/config.hpp"


namespace level = spdlog::level;

auto APIENTRY DllMain(HMODULE _module, DWORD _call_reason, [[maybe_unused]] LPVOID _reserved) -> BOOL
{
  switch (_call_reason)
  {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(_module);

    const auto config{ utils::config::load() };

    if (config["logger"]["use_console"].value_or(utils::config::default_use_log_console())) {
      AllocConsole();
    }

    spdlog::set_default_logger(spdlog::stdout_color_mt<spdlog::async_factory>("loader", spdlog::color_mode::always));
    spdlog::set_level(level::from_str(config["logger"]["level"].value_or(utils::config::default_log_level())));

    std::thread{ &lua100::plugins::attach, &lua100::plugins::instance }.detach();

    // TODO: hook main of game and wait plugins loading

    break;
  }
  }
  return TRUE;
}