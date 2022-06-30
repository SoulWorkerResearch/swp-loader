// local
#include "utils/logger.hpp"

namespace zpp = libzippp;
namespace fs = std::filesystem;

auto rename_exists(const std::string& _path) -> void
{
  if (fs::exists(_path)) {
    const auto time{ fs::last_write_time(_path) };
    const auto name{ std::format("{:%FT%H-%M-%S}.zip", time) };

    fs::rename(_path, fs::path{ _path }.replace_filename(name));
  }
}

auto lua100::utils::logger::pack(const std::string& _directory) -> void
{
  const auto full_path{ std::format("{}.zip", _directory) };
  rename_exists(full_path);

  zpp::ZipArchive archive{ full_path };
  archive.open(zpp::ZipArchive::New);

  for (const auto& _entry : fs::directory_iterator{ _directory }) {
    const auto path{ _entry.path() };
    auto name = path.filename().string();
    archive.addFile(name, path.string());
  }
}

auto lua100::utils::logger::system_error(void) -> void 
{
  spdlog::error(std::system_category().message(GetLastError()));
}