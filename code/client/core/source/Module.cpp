#ifdef SOUP_BUILD
module;
#endif

// TODO: Add a converter level to Opal?
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <any>
#include <array>
#include <chrono>
#include <codecvt>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <map>
#include <regex>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#ifdef _WIN32

#include <format>

#include <Windows.h>
#undef max
#undef min
#undef CreateDirectory
#undef CreateProcess
#undef GetCurrentTime
#undef GetClassName

#elif defined(__linux__)

#include <spawn.h>
#include <sys/wait.h>

#endif

#ifdef SOUP_BUILD

// TODO module
// TODO: Treat wren as C code
#include "wren/wren.h"

export module Soup.Core;

import reflex;
import CryptoPP;
import Monitor.Host;
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

#include "Utilities/Path.h"
#include "Utilities/SemanticVersion.h"
#include "IO/SystemConsoleManager.h"
#include "Logger/Log.h"
#include "Logger/ConsoleTraceListener.h"
#include "System/LinuxProcessManager.h"
#include "System/STLFileSystem.h"
#include "System/STLSystem.h"

using namespace Opal;

// import CryptoPP
#include "Interface.h"

// import Monitor.Host
#include "Linux/LinuxMonitorProcessManager.h"

// import Wren
#include "wren/wren.h"

#endif

#define CLIENT_CORE_IMPLEMENTATION

#include "utilities/SequenceMap.h"
#include "build/RecipeBuildLocationManager.h"
#include "build/BuildEngine.h"
#include "local-user-config/LocalUserConfigExtensions.h"
#include "package/PackageManager.h"
#include "wren/WrenHost.h"
#include "wren/WrenValueTable.h"