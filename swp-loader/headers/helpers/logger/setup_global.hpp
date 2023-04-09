#pragma once

#include "config.hpp"

namespace swpl::helpers::logger
{
	auto setup_global(const HINSTANCE _instance, const config& _config) -> bool;
}
