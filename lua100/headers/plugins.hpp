#pragma once

// windows
#include <Windows.h>

// deps
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// cpp
#include <system_error>
#include <filesystem>
#include <ranges>
#include <vector>

// local
#include "./win/dll.hpp"

// local deps
#include <sdk/plugin/plugin_emplace.hpp>
#include <sdk/plugin/plugin_ready.hpp>

template<typename TStream>
TStream& operator<<(TStream& _os, const std::filesystem::directory_entry& _value)
{
  return _os << _value.path().c_str();
}

namespace lua100
{
  class
  {
  public:
    auto attach(void) -> void
    {
      using std::filesystem::recursive_directory_iterator;
      using std::filesystem::directory_entry;
      using std::filesystem::exists;
      using std::filesystem::create_directory;
      using std::ranges::views::filter;
      using spdlog::stdout_color_mt;
      using spdlog::async_factory;
      using spdlog::color_mode;
      using spdlog::logger;
      using std::shared_ptr;
      using std::exit;

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
          logger->error("{}", std::system_category().message(GetLastError()));
          continue;
        }

        using emplace_ptr_t = decltype(sdk::plugin::emplace)*;
        const auto emplace{ (emplace_ptr_t)GetProcAddress(static_cast<HMODULE>(plugin), "emplace") };
        if (not emplace) {
          logger->error("haven't emplace function.", entry);
          continue;
        }

        using ready_ptr_t = decltype(sdk::plugin::ready)*;
        const auto ready{ (ready_ptr_t)GetProcAddress(static_cast<HMODULE>(plugin), "ready") };
        if (not ready) {
          logger->error("haven't ready function.", entry);
          continue;
        }

        const std::unique_ptr<sdk::plugin::info> info{ emplace() };
        if (not info) {
          logger->error("can't get info.", entry);
          continue;
        }

        // TODO: check game version

        m_plugins.emplace_back(std::forward<win::dll>(plugin));

        ready(logger, info->instance);

        logger->info("loaded", entry);
      }
    }

  private:
    static auto is_dll(const std::filesystem::directory_entry& _value) -> bool
    {
      return _value.path().extension() == ".dll";
    }

    static auto directory(void) -> std::filesystem::path
    {
      using std::filesystem::current_path;
      return current_path() / "plugins";
    }

    std::vector<std::shared_ptr<spdlog::logger>> m_loggers;
    std::vector<win::dll> m_plugins;
  } plugins;
}