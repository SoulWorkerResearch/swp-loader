// local
#include "../headers/exports.hpp"

// deps
#include <spdlog/spdlog.h>

namespace lua100
{
  void lua_atprint([[maybe_unused]] lua_State* _state, [[maybe_unused]] uintptr_t _unk_ptr)
  {
    spdlog::warn("lua_atprint called");
  }
}

// https://youtu.be/SdxsT40DaCg?list=PLJ4jpRq1tGIlNAo3AblOFj9ovgfyp0xQQ
