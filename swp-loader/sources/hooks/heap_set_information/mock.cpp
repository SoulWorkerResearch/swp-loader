#include "hooks/heap_set_information/mock.hpp"
#include "hooks/heap_set_information/uninstall.hpp"
#include "hooks/get_current_process_id/install.hpp"

namespace swpl::hooks::heap_set_information
{
	BOOL WINAPI mock(_In_opt_ HANDLE HeapHandle, _In_ HEAP_INFORMATION_CLASS HeapInformationClass, _In_reads_bytes_opt_(HeapInformationLength) PVOID HeapInformation, _In_ SIZE_T HeapInformationLength)
	{
		using namespace swpsdk::detour;

		if (not transaction({ uninstall, get_current_process_id::install })) {
			std::exit(-1);
		}

		return HeapSetInformation(HeapHandle, HeapInformationClass, HeapInformation, HeapInformationLength);
	}
}
