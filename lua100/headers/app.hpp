#pragma once

// local
#include "plugins.hpp"

namespace lua100
{
	class app final : public swpsdk::utils::memory<app>
	{
	public:

#pragma region Methods

		auto install(void) -> void;
		auto detach(void) -> void;

#pragma endregion Methods

#pragma region Methods

		app(void);

#pragma endregion Methods

	private:

#pragma region Members

		plugins m_plugins{};

		inline static const std::string m_logs_path{ std::format("logs/latest", std::chrono::system_clock::now()) };

#pragma endregion Members

#pragma region Methods

		auto attach(void) -> void;

		static DWORD WINAPI on_attach_requested(VOID) noexcept;

#pragma endregion Methods

	};
}