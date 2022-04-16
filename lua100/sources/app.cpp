// local
#include "../headers/app.hpp"
#include "../headers/utils/config.hpp"
#include "../headers/utils/logger.hpp"
#include "../headers/logger_factory.hpp"

// cpp
#include <memory>
#include <thread>

namespace level = spdlog::level;
namespace fs = std::filesystem;

namespace lua100
{
  auto setup_logger(const HMODULE _module, const std::shared_ptr<logger_factory>& _factory) -> void
  {
    std::string name(MAX_PATH, '\0');
    GetModuleFileNameA(_module, name.data(), name.size());

    const auto logger{ _factory->create(name) };
    spdlog::set_default_logger(logger);
  }

  auto app::attach(const HMODULE _module) -> void
  {
    const auto config{ utils::config::load() };
    const auto use_console{ config["logger"]["use_console"].value_or(utils::config::default_use_log_console()) };
    const auto log_level{ config["logger"]["level"].value_or(utils::config::default_log_level()) };

    if (use_console) {
      AllocConsole();
    }

    const auto factory{ std::make_shared<logger_factory>(m_logs_path, log_level, use_console) };
    setup_logger(_module, factory);

    std::thread{ &lua100::plugins::attach, &m_plugins, factory }.detach();

    // TODO: hook main of game and wait plugins loading
  }

  auto app::detach(void) -> void
  {
    m_plugins.detach();

    spdlog::shutdown();

    utils::logger::pack(m_logs_path);
    fs::remove_all(m_logs_path);
  }

  app::app(void) 
  {
    fs::create_directories(m_logs_path);
  }
}