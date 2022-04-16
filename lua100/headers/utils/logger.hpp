#pragma once

// cpp
#include <filesystem>

// local
#include "../logger_factory.hpp"

namespace lua100::utils::logger
{
  auto pack(const std::string& _path) -> void;
}
