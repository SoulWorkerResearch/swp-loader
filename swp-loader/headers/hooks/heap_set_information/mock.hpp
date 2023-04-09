#pragma once

namespace swpl::hooks::heap_set_information 
{
	BOOL WINAPI mock(_In_opt_ HANDLE HeapHandle, _In_ HEAP_INFORMATION_CLASS HeapInformationClass, _In_reads_bytes_opt_(HeapInformationLength) PVOID HeapInformation, _In_ SIZE_T HeapInformationLength);
}
