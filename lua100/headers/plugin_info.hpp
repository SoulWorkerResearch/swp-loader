#pragma once

// local
#include "./win/dll.hpp"

// deps
#include <spdlog/spdlog.h>

namespace lua100
{
  struct plugin_info sealed
  {
#pragma region Methods

    auto dll(void) const noexcept -> const win::dll& { return m_dll; }
    auto logger(void) const noexcept -> const std::shared_ptr<spdlog::logger> { return m_logger; }

#pragma endregion Methods

#pragma region Constructors

    plugin_info(win::dll&& _dll, std::shared_ptr<spdlog::logger>&& _logger)
      : m_dll{ std::forward<decltype(_dll)>(_dll) }
      , m_logger{ _logger }
    {
    }

#pragma endregion Constructors

  private:

#pragma region Propertyies

    win::dll m_dll;
    std::shared_ptr<spdlog::logger> m_logger;

#pragma endregion Propertyies

  };
}