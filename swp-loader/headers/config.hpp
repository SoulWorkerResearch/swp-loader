#pragma once

namespace swpl
{
	struct config final
	{
		bool use_console{ default_use_console() };
		std::string level{ default_log_level() };

		static config create(const std::filesystem::path _path = TEXT("configs/loader.json"))
		{
			using namespace nlohmann;

			if (std::ifstream file{ _path }; file) {
				return json::parse(file).get<config>();
			}

			auto json = R"(
{
	"use_console": false,
	"level": "off"
}
)";

			std::ofstream{ _path } << json;

			return {};
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(config, use_console, level);

	private:

		constexpr auto default_use_console(void) -> bool
		{
#ifdef _DEBUG
			return true;
#else
			return false;
#endif // _DEBUG
		}

		constexpr auto default_log_level(void) -> std::string_view
		{
#ifdef _DEBUG
			return std::string_view{ SPDLOG_LEVEL_NAME_TRACE };
#else
			return std::string_view{ SPDLOG_LEVEL_NAME_OFF };
#endif // _DEBUG
		}
		};
	}
