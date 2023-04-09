#pragma once

namespace swpl::plugin
{
	class loader final
	{
	public:
		auto try_resolve(const boost::dll::shared_library& _plugin) const -> int32_t;

		auto try_attach(const std::filesystem::directory_entry& _entry) const->std::expected<boost::dll::shared_library, int32_t>;

		loader(void);

	private:
		const swpsdk::version m_game_version;
	};
}
