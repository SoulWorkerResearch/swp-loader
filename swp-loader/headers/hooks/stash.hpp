#pragma once

namespace swpl::hooks
{
	template<typename TPtr>
	struct stash final
	{
		inline static TPtr* value{ nullptr };
	};
}
