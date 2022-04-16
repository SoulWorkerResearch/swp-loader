#pragma once

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

namespace lua100
{
  LUALIB_API void __declspec(dllexport) lua_atprint([[maybe_unused]] lua_State* _state, [[maybe_unused]] uintptr_t _unk_ptr);
}
