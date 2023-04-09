#include "hooks/get_current_process_id/install.hpp"
#include "hooks/get_current_process_id/ptr.hpp"
#include "hooks/get_current_process_id/mock.hpp"

namespace swpl::hooks::get_current_process_id
{
	using namespace swpsdk::detour;

	auto uninstall(void) -> bool
	{
		return detach(ptr, mock, "GetCurrentProcessId");
	}
}
