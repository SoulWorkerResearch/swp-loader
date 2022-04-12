// windows
#include <Windows.h>
#include <Psapi.h>

// cpp
#include <filesystem>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <array>

// deps
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <toml++/toml.h>

// local
#include "../headers/plugins.hpp"
#include "../headers/utils/config.hpp"
#include "../headers/logger_factory.hpp"

namespace level = spdlog::level;
namespace fs = std::filesystem;

using namespace lua100;

auto APIENTRY DllMain(HMODULE _module, DWORD _call_reason, [[maybe_unused]] LPVOID _reserved) -> BOOL
{
  switch (_call_reason)
  {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(_module);

    const auto config{ utils::config::load() };
    const auto use_console{ config["logger"]["use_console"].value_or(utils::config::default_use_log_console()) };
    const auto log_level{ config["logger"]["level"].value_or(utils::config::default_log_level()) };

    if (use_console) {
      AllocConsole();
    }

    std::string name(MAX_PATH, '\0');
    GetModuleFileNameA(_module, name.data(), name.size());

    const auto lf{ std::make_shared<logger_factory>(log_level, use_console) };

    const auto logger{ lf->create(name) };
    spdlog::set_default_logger(logger);
    
    std::thread{ &lua100::plugins::attach, &lua100::plugins::instance, lf }.detach();

    // TODO: hook main of game and wait plugins loading

    break;
  }
  case DLL_PROCESS_DETACH:
    spdlog::shutdown();
    break;
  }
  return TRUE;
}
