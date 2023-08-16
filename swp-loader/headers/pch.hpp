// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define SPDLOG_USE_STD_FORMAT
#define BOOST_DLL_USE_STD_FS

// add headers that you want to pre-compile here
#include "framework.hpp"

#include <swpsdk/targetver.hpp>
#include <swpsdk/plugin/info.hpp>
#include <swpsdk/plugin/get_info.hpp>

#include <swpsdk/fmt-formatters/directory_entry.hpp>
#include <swpsdk/fmt-formatters/path.hpp>
#include <swpsdk/fmt-formatters/version.hpp>

#include <swpsdk/detour/transaction.hpp>
#include <swpsdk/detour/attach.hpp>
#include <swpsdk/detour/detach.hpp>

#include <swpsdk/sdk_version.hpp>
#include <swpsdk/version.hpp>
#include <swpsdk/helpers/memory.hpp>
#include <swpsdk/extensions/spdlog/system_error.hpp>
#include <swpsdk/parsers/from_pair.hpp>
#include <swpsdk/configs/ver.hpp>

#include <detours/detours.h>
#include <nlohmann/json.hpp>
#include <boost/dll.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <expected>

#endif //PCH_H
