#pragma once

// deps
#include <spdlog/logger.h>

// cpp
#include <ranges>
#include <vector>

// local
#include "./win/dll.hpp"
#include "./logger_factory.hpp"

namespace lua100
{
  class plugins final
  {
  public:
    using plugins_t = std::vector<win::dll>;

    static plugins instance;

    auto attach(const logger_factory_t& _logger) -> void;

  private:
    plugins(void) noexcept {}

    [[nodiscard]]
    static auto is_dll(const std::filesystem::directory_entry& _value) -> bool;

    [[nodiscard]]
    static auto directory(void)->std::filesystem::path;

    plugins_t m_plugins;
  };
}