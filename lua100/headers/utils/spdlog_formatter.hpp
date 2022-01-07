#pragma once

// deps
#include <spdlog/fmt/ostr.h>
#include <neargye/semver.hpp>

template<typename OStream>
OStream& operator<<(OStream& _stream, const semver::version& _value) 
{
  return _stream << _value;
}
