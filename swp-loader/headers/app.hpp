#pragma once

#include <swpsdk/helpers/memory.hpp>

namespace swpl
{
	class app final : public swpsdk::helpers::memory<app>
	{
	public:
		inline static std::filesystem::path LOGS_DIR{ "logs" };
		inline static std::filesystem::path PLUGINS_DIR{ "plugins" };

		inline static std::string_view PLUGIN_EXT{ ".dll" };

	public:
		auto on_attach(const HINSTANCE _instance) -> bool;
		auto on_detach(void) -> bool;

		auto load_plugins(void) -> void;

		app(void) = default;

	private:
		std::vector<boost::dll::shared_library> m_plugins{};
	};
}
