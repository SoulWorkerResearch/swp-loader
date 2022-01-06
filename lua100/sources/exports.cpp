#include "../headers/exports.hpp"
#include <spdlog/spdlog.h>

void lua_atprint([[maybe_unused]] lua_State* _state, [[maybe_unused]] uintptr_t _unk_ptr)
{
  spdlog::warn("lua_atprint called");
}
