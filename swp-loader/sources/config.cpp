#pragma once

#include "config.hpp"
#include "app.hpp"

namespace swpl
{
	auto config::create(void) -> config
	{
		using namespace nlohmann;

		const auto path{ app::CONFIGS_DIR / "loader.json" };

		if (std::ifstream file{ path }; file) {
			return json::parse(file).get<config>();
		}

		config cfg{};

		if (auto file{ std::ofstream{ path } }; file) {
			// braces - https://github.com/nlohmann/json/issues/2204
			file << json(cfg).dump(2);
		}

		return cfg;
	}
}
