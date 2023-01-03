// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <stdexcept>

#ifdef SOUP_BUILD

import Opal;

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

#endif

#include "GenerateEngine.h"

int main()
{
	try
	{
		GenerateEngine::Run();
	}
	catch (const std::exception& ex)
	{
		Log::Error(ex.what());
		return -1;
	}
}