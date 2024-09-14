#pragma once

#include <swpsdk/helpers/memory.hpp>
#include "config.hpp"

namespace swpl
{
	class app final : public swpsdk::helpers::memory<app>
	{
	public:
		inline static std::filesystem::path ROOT_DIR{ "swp" };

		inline static std::filesystem::path LOGS_DIR{ ROOT_DIR / "logs" };
		inline static std::filesystem::path PLUGINS_DIR{ ROOT_DIR / "plugins" };
		inline static std::filesystem::path CONFIGS_DIR{ ROOT_DIR / "configs" };

		inline static std::string_view PLUGIN_EXT{ ".dll" };

	public:
		auto on_attach(const HINSTANCE _instance) -> bool;
		auto on_detach(void) -> bool;

		auto load_plugins(void) -> void;

		app(void);

	private:
		std::vector<boost::dll::shared_library> m_plugins{};
		config m_config;
	};
}
