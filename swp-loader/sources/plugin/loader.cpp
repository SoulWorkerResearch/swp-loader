#include "plugin/loader.hpp"

namespace dll = boost::dll;
namespace fs = std::filesystem;

namespace instance = swpsdk::plugin;

namespace swpl::plugin
{
	auto loader::try_resolve(const boost::dll::shared_library& _plugin) const -> int32_t
	{
		using get_info_ptr_t = decltype(instance::get_info);
		const auto get_info{ _plugin.get<get_info_ptr_t>("get_info") };

		instance::info info{};
		std::invoke(get_info, info);

		using loader_t = instance::loader;

		auto p{ reinterpret_cast<const loader_t*>(info.instance) };
		return std::invoke(&loader_t::internal_main, p);
	}

	auto loader::try_attach(const fs::directory_entry& _entry) const -> std::expected<boost::dll::shared_library, int32_t>
	{
		auto logger{ spdlog::default_logger()->clone(_entry.path().stem().string()) };

		logger->debug("try load plugin: {}", _entry.path().filename());
		dll::shared_library dll{};
		dll.load(_entry.path());

		if (not dll.has("get_info")) {
			logger->error("haven't get_info function.", _entry);
			return std::unexpected{ EXIT_FAILURE };
		}

		using get_info_ptr_t = decltype(instance::get_info);
		const auto get_info{ dll.get<get_info_ptr_t>("get_info") };

		instance::info info{};
		std::invoke(get_info, info);

		if (nullptr == info.instance) {
			logger->critical("instance ptr is nullptr");
			return std::unexpected{ EXIT_FAILURE };
		}

		logger->debug("plugin target game version: {}", info.game_version);
		if (m_game_version != info.game_version) {
			logger->warn("mismatch game version [plugin: {}] != [game: {}]", info.game_version, m_game_version);
		}

		using get_version_t = decltype(swpsdk::plugin::get_version);
		const auto get_version{ dll.get<get_version_t>("get_version") };

		swpsdk::version sdk_version{};
		std::invoke(get_version, sdk_version);

		logger->debug("plugin target sdk version: {}", sdk_version);
		if (swpsdk::sdk_version != sdk_version) {
			logger->warn("mismatch sdk version [plugin: {}] != [loader: {}]", sdk_version, swpsdk::sdk_version);
		}

		using loader_t = instance::loader;

		auto p{ reinterpret_cast<const loader_t*>(info.instance) };
		if (const auto r{ std::invoke(&loader_t::internal_attach, p, logger) }; r != EXIT_SUCCESS) {
			logger->info("failed to attach");
			return std::unexpected{ EXIT_FAILURE };
		}

		logger->info("attached v{}", info.plugin_version);
		return dll;
	}

	loader::loader(void) : m_game_version{ swpsdk::parsers::from_pair<swpsdk::configs::ver::pair>(fs::current_path() / "Ver.ini").client.version }
	{
		spdlog::info("sdk v{}", swpsdk::sdk_version);
		spdlog::info("game v{}", m_game_version);
	}
}
