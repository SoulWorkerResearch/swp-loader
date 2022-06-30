// local
#include "app.hpp"
#include "utils/config.hpp"
#include "utils/logger.hpp"
#include "logger_factory.hpp"

namespace level = spdlog::level;
namespace fs = std::filesystem;

decltype(GetCurrentProcessId)* TrueGetCurrentProcessId = reinterpret_cast<decltype(TrueGetCurrentProcessId)>(GetProcAddress(GetModuleHandle(TEXT("Kernel32.dll")), "GetCurrentProcessId"));

namespace lua100
{
	auto setup_logger(const toml::table& _config, const std::string& _path) -> const logger_factory::type_value
	{
		const auto use_console{ _config["logger"]["use_console"].value_or(utils::config::default_use_log_console()) };
		const auto log_level{ _config["logger"]["level"].value_or(utils::config::default_log_level()) };

		if (use_console && 0 == AllocConsole()) {
			throw std::runtime_error("AllocConsole");
		}

		const auto factory{ std::make_shared<logger_factory>(_path, log_level, use_console) };

		std::wstring name(MAX_PATH, '\0');
		if (0 == GetModuleFileName(GetModuleHandle(TEXT(MODULE_NAME)), name.data(), name.size())) {
			throw std::runtime_error("GetModuleFileName");
		}

		const auto logger{ factory->create(name) };
		spdlog::set_default_logger(logger);

		return factory;
	}
}

auto lua100::app::install(void) -> void
{
	if (NO_ERROR != DetourTransactionBegin()) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourUpdateThread(GetCurrentThread())) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourAttach(&(PVOID&)TrueGetCurrentProcessId, on_attach_requested)) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourTransactionCommit()) {
		utils::logger::system_error();
	}
}

auto lua100::app::detach(void) -> void
{
	m_plugins.detach();

	spdlog::shutdown();

	utils::logger::pack(m_logs_path);
	fs::remove_all(m_logs_path);
}

lua100::app::app(void)
{
	fs::create_directories(m_logs_path);
}

auto lua100::app::attach(void) -> void
{
	const auto config{ utils::config::load() };
	const auto factory{ setup_logger(config, m_logs_path) };

	m_plugins.attach(factory);
}

DWORD WINAPI lua100::app::on_attach_requested(VOID) noexcept
{
	if (NO_ERROR != DetourTransactionBegin()) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourUpdateThread(GetCurrentThread())) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourDetach(&(PVOID&)TrueGetCurrentProcessId, on_attach_requested)) {
		utils::logger::system_error();
	}

	if (NO_ERROR != DetourTransactionCommit()) {
		utils::logger::system_error();
	}

	instance().attach();

	return TrueGetCurrentProcessId();
}
