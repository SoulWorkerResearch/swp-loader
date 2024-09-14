#pragma once

namespace swpl
{
	struct config final
	{
		struct console final
		{
			/*
			 * Show console
			 */
			bool use{ default_use() };

			/*
			 * Leave console after application exited
			 */
			bool leave{ default_leave() };

			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(console, use, leave);

		private:

			constexpr auto default_use(void) -> bool
			{
#ifdef _DEBUG
				return true;
#else
				return false;
#endif // _DEBUG
			}

			constexpr auto default_leave(void) -> bool
			{
#ifdef _DEBUG
				return true;
#else
				return false;
#endif // _DEBUG
			}
		};

		struct logger final 
		{
			/*
			 * Logger logger_level
			 */
			std::string level{ default_level() };

		private:

			constexpr auto default_level(void) -> std::string_view
			{
#ifdef _DEBUG
				return std::string_view{ SPDLOG_LEVEL_NAME_TRACE };
#else
				return std::string_view{ SPDLOG_LEVEL_NAME_OFF };
#endif // _DEBUG
			}

			NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(logger, level);
		};

		console console{};
		logger logger{};

		static auto create(void) -> config;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(config, console, logger);
	};
}

// https://youtu.be/RXVycJuqIIo