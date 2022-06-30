// local
#include "app.hpp"

namespace level = spdlog::level;
namespace fs = std::filesystem;

using namespace lua100;

auto APIENTRY DllMain(HMODULE _module, DWORD _call_reason, [[maybe_unused]] LPVOID _reserved) -> BOOL
{
  switch (_call_reason)
  {
  case DLL_PROCESS_ATTACH: {
    DisableThreadLibraryCalls(_module);

    app::instance().install();

    break;
  }
  case DLL_PROCESS_DETACH:
    app::instance().detach();

    break;
  }
  return TRUE;
}
