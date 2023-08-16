#include "helpers/logger/setup_global.hpp"
#include "app.hpp"

namespace sinks = spdlog::sinks;
namespace fs = std::filesystem;

namespace swpl::helpers::logger
{
	auto setup_global(const HINSTANCE _instance, const config& _config) -> bool
	{
		constexpr static const auto generate_name = [](void) -> decltype(auto)
		{
			return std::format("{:%FT%H-%M-%S}.txt", std::chrono::system_clock::now());
		};

		if (_config.use_console && FALSE == AllocConsole()) {
			return false;
		}

		std::wstring name(MAX_PATH, TEXT('\0'));
		if (FALSE == GetModuleFileName(_instance, name.data(), static_cast<DWORD>(name.size()))) {
			return false;
		}

		const auto to{ app::LOGS_DIR / std::invoke(generate_name) };
		std::vector<spdlog::sink_ptr> sinks{ std::make_shared<sinks::basic_file_sink_mt>(to.string(), true) };

		if (_config.use_console) {
			sinks.emplace_back(std::make_shared<sinks::stdout_color_sink_mt>(spdlog::color_mode::always));
		}

		spdlog::set_default_logger(std::make_shared<spdlog::logger>(fs::path{ name }.filename().stem().string(), sinks.cbegin(), sinks.cend()));
		spdlog::set_level(spdlog::level::from_str(_config.level));

		return true;
	}
}
