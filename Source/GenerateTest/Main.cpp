// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#ifdef SOUP_BUILD

import Opal;
import Soup.Core;

using namespace Opal;

#else

// import Opal
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <functional>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <locale>
#include <map>
#include <optional>
#include <queue>
#include <sstream>
#include <string>

#include <spawn.h>
#include <sys/wait.h>

#include "Utilities/Path.h"
#include "Utilities/SemanticVersion.h"
#include "IO/SystemConsoleManager.h"
#include "Logger/Log.h"
#include "Logger/ConsoleTraceListener.h"
#include "System/LinuxProcessManager.h"
#include "System/STLFileSystem.h"
#include "System/STLSystem.h"

using namespace Opal;

// import Soup.Core
#include <cstring>
#include <regex>
#include <set>
#include <variant>
#include "Build/BuildEngine.h"
#include "Package/PackageManager.h"

#endif

// TODO import
// TODO: Treat wren as C code
#include "include/wren.h"

#include "GenerateTestHost.h"

int main(int argc, char** argv)
{
	try
	{
		// Setup the filter
		auto defaultTypes =
			static_cast<uint32_t>(TraceEventFlag::Diagnostic) |
			static_cast<uint32_t>(TraceEventFlag::Information) |
			static_cast<uint32_t>(TraceEventFlag::HighPriority) |
			static_cast<uint32_t>(TraceEventFlag::Warning) |
			static_cast<uint32_t>(TraceEventFlag::Error) |
			static_cast<uint32_t>(TraceEventFlag::Critical);
		auto filter = std::make_shared<EventTypeFilter>(
				static_cast<TraceEventFlag>(defaultTypes));

		// Setup the console listener
		Log::RegisterListener(
			std::make_shared<ConsoleTraceListener>(
				"Log",
				filter,
				false,
				false));

		// Setup the real services
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());

		Log::Diag("ProgramStart");

		if (argc < 2 || argc > 3)
		{
			Log::Error("Invalid parameters. Expected one or two parameter.");
			return -1;
		}

		auto scriptFile = Path(argv[1]);

		std::optional<Path> bundlesFile;
		if (argc > 2)
		{
			bundlesFile = Path(argv[2]);
		}

		auto host = std::make_unique<Soup::Core::Generate::GenerateTestHost>(
			std::move(scriptFile),
			std::move(bundlesFile));
		host->InterpretMain();
	}
	catch (const std::exception& ex)
	{
		Log::Error(ex.what());
		return -1;
	}
}