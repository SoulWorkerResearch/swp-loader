// local
#include "../../headers/utils/game_version.hpp"

// windows
#include <Windows.h>

// cpp
#include <filesystem>

using namespace swpsdk::game;
using namespace std;
using namespace std::filesystem;

auto lua100::utils::game_version::read(void) -> version
{
  const auto path{ current_path() / "Ver.ini" };

  char buffer[MAX_PATH]{};
  auto length{ GetPrivateProfileStringA("Client", "ver", "0.0.0.0", buffer, MAX_PATH, path.string().c_str()) };

  return version::from_chars({ buffer, length });
}