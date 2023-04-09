#include "app.hpp"
#include "config.hpp"
#include "plugin/loader.hpp"
#include "hooks/heap_set_information/install.hpp"
#include "helpers/logger/setup_global.hpp"

namespace fs = std::filesystem;
namespace views = std::views;

namespace swpl
{
	auto ensure_dirs_exists(void) -> bool
	{
		if (not fs::exists(app::LOGS_DIR) and not fs::create_directories(app::LOGS_DIR)) {
			return false;
		}

		if (not fs::exists(app::PLUGINS_DIR) and not fs::create_directories(app::PLUGINS_DIR)) {
			return false;
		}

		return true;
	}

	auto app::on_attach(const HINSTANCE _instance) -> bool
	{
		using namespace swpsdk::detour;

		if (not ensure_dirs_exists()) {
			return false;
		}

		if (const auto cfg{ config::create() }; not helpers::logger::setup_global(_instance, cfg)) {
			return false;
		}

		if (not transaction({ hooks::heap_set_information::install })) {
			return false;
		}

		return true;
	}

	auto app::on_detach(void) -> bool
	{
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
}
