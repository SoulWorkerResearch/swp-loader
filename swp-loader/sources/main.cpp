// main.cpp : Defines the entry point for the application.
//

#include "app.hpp"

using namespace spdlog;
using namespace std::filesystem;

BOOL WINAPI DllMain(HINSTANCE _instance, DWORD fdwReason, LPVOID lpvReserved)
{
	if (DetourIsHelperProcess()) { return TRUE; }

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();
		DisableThreadLibraryCalls(_instance);

		return swpl::app::instance().on_attach(_instance);

	case DLL_PROCESS_DETACH:
		if (lpvReserved != nullptr) { break; }

		FreeConsole();

		return swpl::app::instance().on_detach();
	}

	return TRUE;
}
