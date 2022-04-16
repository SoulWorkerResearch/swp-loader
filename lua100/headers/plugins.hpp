#pragma once

// deps
#include <spdlog/logger.h>

// cpp
#include <ranges>
#include <vector>

// local
#include "./win/dll.hpp"
#include "./logger_factory.hpp"
#include "./plugin_info.hpp"

namespace lua100
{
  class plugins final
  {
  public:

#pragma region Types

    using plugins_t = std::vector<std::unique_ptr<plugin_info>>;

#pragma endregion Types

#pragma region Methods

    auto attach(const logger_factory_t& _logger) -> void;

    constexpr auto detach(void) -> void
    {
      m_plugins.clear();
    }

    [[nodiscard]]
    constexpr plugins_t::iterator begin(void) noexcept { return m_plugins.begin(); }

    [[nodiscard]]
    constexpr plugins_t::iterator end(void) noexcept { return m_plugins.end(); }

    [[nodiscard]]
    constexpr plugins_t::const_iterator cbegin(void) const noexcept { return m_plugins.cbegin(); }

    [[nodiscard]]
    constexpr plugins_t::const_iterator cend(void) const noexcept { return m_plugins.cend(); }

#pragma endregion Methods

  private:

#pragma region Methods

    [[nodiscard]]
    static auto is_dll(const std::filesystem::directory_entry& _value) -> bool;

    [[nodiscard]]
    static auto directory(void)->std::filesystem::path;

#pragma endregion Methods

#pragma region Members

    plugins_t m_plugins;

#pragma endregion Members

  };
}