#include "hooks/get_current_process_id/mock.hpp"
#include "hooks/get_current_process_id/uninstall.hpp"
#include "hooks/get_current_process_id/install.hpp"

#include "app.hpp"

namespace swpl::hooks::get_current_process_id
{
	DWORD WINAPI mock(VOID)
	{
		using namespace swpsdk::detour;

		if (not transaction({ uninstall })) {
			std::exit(-1);
		}

		app::instance().load_plugins();

		return GetCurrentProcessId();
	}
}
