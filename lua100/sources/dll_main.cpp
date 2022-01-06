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

// local
#include "../headers/plugins.hpp"

auto APIENTRY DllMain(HMODULE _module, DWORD _call_reason, LPVOID _reserved) -> BOOL
{
  switch (_call_reason)
  {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(_module);
    AllocConsole();

    std::atexit([]
    {
      spdlog::drop_all();
      system("pause");
    });

    spdlog::set_default_logger(spdlog::stdout_color_mt<spdlog::async_factory>("loader", spdlog::color_mode::always));
    spdlog::set_level(spdlog::level::debug);

    std::thread([] {
      lua100::plugins::instance.attach();
    }).detach();

    break;
  }
  }
  return TRUE;
}