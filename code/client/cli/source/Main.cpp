// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef SOUP_BUILD

import Monitor.Host;
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

#include "utilities/Path.h"
#include "utilities/SemanticVersion.h"
#include "io/SystemConsoleManager.h"
#include "logger/Log.h"
#include "logger/ConsoleTraceListener.h"
#include "system/LinuxProcessManager.h"
#include "system/STLFileSystem.h"
#include "system/STLSystem.h"

using namespace Opal;

// import CryptoPP
#include "Interface.h"

// import Monitor.Host
#include "linux/LinuxMonitorProcessManager.h"

// import Soup.Core
#include <cstring>
#include <regex>
#include <set>
#include <variant>
#include "build/BuildEngine.h"
#include "package/PackageManager.h"

#endif

#include "Program.h"

int main(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	auto program = Soup::Client::Program();
	return program.Run(std::move(args));
}
