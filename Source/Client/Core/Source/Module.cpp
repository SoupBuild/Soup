#ifdef SOUP_BUILD
module;
#endif

// TODO: Add a converter level to Opal?
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <any>
#include <array>
#include <chrono>
#include <codecvt>
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
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#ifdef _WIN32

#include <Windows.h>
#undef max
#undef min
#undef CreateProcess
#undef GetCurrentTime

#endif

#ifdef SOUP_BUILD
export module Soup.Core;

import reflex;
import CryptoPP;
import Monitor.Host;
import Opal;

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
#include "System/STLFileSystem.h"
#include "System/STLSystem.h"

#endif

using namespace Opal;

#include "Build/RecipeBuildLocationManager.h"
#include "Build/BuildEngine.h"
#include "LocalUserConfig/LocalUserConfigExtensions.h"
#include "Package/PackageManager.h"