// pch.hpp: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

// windows
#include <Windows.h>
#include <Psapi.h>

// cpp
#include <string>
#include <filesystem>
#include <string_view>
#include <memory>
#include <vector>
#include <ranges>
#include <format>
#include <fstream>
#include <system_error>
#include <future>
#include <thread>

// local
#include "swpsdk/defines.hpp"
#include "swpsdk/version.hpp"
#include "swpsdk/utils/memory.hpp"
#include "swpsdk/utils/spdlog_formatter.hpp"
#include "swpsdk/plugin/attach.hpp"
#include "swpsdk/plugin/loader.hpp"

// vcpkg
#include <toml++/toml.h>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <detours/detours.h>
#include <libzippp/libzippp.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#endif //PCH_H
