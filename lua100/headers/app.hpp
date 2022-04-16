#pragma once

// local deps
#include <swpsdk/utils/memory.hpp>

// local
#include "plugins.hpp"

namespace lua100
{
  class app final : public swpsdk::utils::memory<app>
  {
  public:

#pragma region Methods

    auto attach(const HMODULE _module) -> void;

    auto detach(void) -> void;

#pragma endregion Methods

#pragma region Methods

    app(void);

#pragma endregion Methods

  private:

#pragma region Members

    plugins m_plugins;
    const std::string m_logs_path{ std::format("logs/latest", std::chrono::system_clock::now()) };

#pragma endregion

  };
}