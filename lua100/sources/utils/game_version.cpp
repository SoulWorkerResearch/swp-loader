// local
#include "utils/game_version.hpp"

using namespace swpsdk;
using namespace std;
using namespace std::filesystem;

auto lua100::utils::game_version::read(void) -> version
{
  const auto path{ current_path() / "Ver.ini" };

  char buffer[MAX_PATH]{};
  const auto length{ GetPrivateProfileStringA("Client", "ver", "0.0.0.0", buffer, MAX_PATH, path.string().c_str()) };

  return version::from_chars({ buffer, length });
}