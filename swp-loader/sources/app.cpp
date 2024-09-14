#include "app.hpp"
#include "config.hpp"
#include "plugin/loader.hpp"
#include "hooks/heap_set_information/install.hpp"
#include "helpers/logger/setup_global.hpp"

namespace fs = std::filesystem;
namespace views = std::views;

namespace swpl
{
	static auto ensure_dirs_exists(void) -> bool
	{
		const std::initializer_list dirs{ app::ROOT_DIR, app::LOGS_DIR, app::LOGS_DIR, app::CONFIGS_DIR };

		return not std::ranges::any_of(dirs, [](auto v) 
			{
				return not fs::exists(v) and not fs::create_directories(v); 
			});
	}

	auto app::on_attach(const HINSTANCE _instance) -> bool
	{
		using namespace swpsdk::detour;

		if (not ensure_dirs_exists()) {
			return false;
		}

		if (not helpers::logger::setup_global(_instance, m_config)) {
			return false;
		}

		if (not transaction({ hooks::heap_set_information::install })) {
			return false;
		}

		return true;
	}

	auto app::on_detach(void) -> bool
	{
		spdlog::debug("Logger shutdown.");
		spdlog::shutdown();

		if (m_config.console.use && not m_config.console.leave) {
			FreeConsole();
		}

		return true;
	}

	auto app::load_plugins(void) -> void
	{
		spdlog::debug("loading plugins...");

		constexpr const auto is_dll = [](const fs::directory_entry& _value) -> bool
		{
			return _value.path().extension() == PLUGIN_EXT;
		};

		constexpr const auto plugins_directory = [](void) -> fs::path
		{
			return fs::current_path() / PLUGINS_DIR;
		};

		plugin::loader loader{};
		
		m_plugins.append_range(
			fs::recursive_directory_iterator{ plugins_directory() } |
			views::filter(is_dll) |
			views::transform([&](auto&& e) { return loader.try_attach(e); }) |

			// without it the filter will cause double loading of the plugins.
			// this is a STL bug or i don't understand something?
			std::ranges::to<std::vector>() |
			
			views::filter([](auto&& e) { return e.has_value(); }) |
			views::transform([](auto&& e) { return e.value(); }));

		for (const auto& plugin : m_plugins) {
			loader.try_resolve(plugin);
		}

		spdlog::debug("plugins loaded");
	}

	app::app(void) : m_config { config::create() }
	{
	}
}
