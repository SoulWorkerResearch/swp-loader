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
    std::string logs_path;

#pragma endregion

  };
}