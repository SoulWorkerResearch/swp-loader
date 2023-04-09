#include "hooks/heap_set_information/install.hpp"
#include "hooks/heap_set_information/ptr.hpp"
#include "hooks/heap_set_information/mock.hpp"

namespace swpl::hooks::heap_set_information
{
	using namespace swpsdk::detour;

	auto uninstall(void) -> bool
	{
		return detach(ptr, mock, "HeapSetInformation");
	}
}
