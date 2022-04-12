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

auto APIENTRY DllMain(HMODULE _module, DWORD _call_reason, [[maybe_unused]] LPVOID _reserved) -> BOOL
{
  namespace slevel = spdlog::level;
  namespace fs = std::filesystem;

#ifdef _DEBUG
  std::string log_level{ SPDLOG_LEVEL_NAME_DEBUG };
  bool log_console = true;
#else
  std::string log_level{ SPDLOG_LEVEL_NAME_OFF };
  bool log_console = false;
#endif // _DEBUG

  switch (_call_reason)
  {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(_module);

    auto&& config{ fs::exists("configs/loader.yml") ? toml::parse_file("configs/loader.yml") : toml::parse_result{} };
    
    if (config["logger"]["use_console"].value_or<bool>(std::forward<bool>(log_console))) {
      AllocConsole();
    }

    spdlog::set_default_logger(spdlog::stdout_color_mt<spdlog::async_factory>("loader", spdlog::color_mode::always));
    spdlog::set_level(slevel::from_str(config["logger"]["level"].value_or<std::string>(std::forward<std::string>(log_level))));

    std::thread{ &lua100::plugins::attach, &lua100::plugins::instance }.detach();

    // TODO: hook main of game and wait plugins loading

    break;
  }
  }
  return TRUE;
}