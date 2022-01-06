#pragma once

// deps
#include <spdlog/logger.h>

// cpp
#include <ranges>
#include <vector>

// local
#include "./win/dll.hpp"

namespace lua100
{
  class plugins final
  {
  public:
    static plugins instance;

    auto attach(void) -> void;

  private:
    plugins(void) noexcept {}

    static auto is_dll(const std::filesystem::directory_entry& _value) -> bool;
    static auto directory(void)->std::filesystem::path;

    std::vector<std::shared_ptr<spdlog::logger>> m_loggers;
    std::vector<win::dll> m_plugins;
  };
}