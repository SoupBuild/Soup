module;

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
#include <vector>

#ifdef WIN32
#include <Windows.h>

#undef max
#undef min
#undef CreateProcess
#endif

export module Soup.Core;

import CryptoPP;
import json11;
import toml11;
import Monitor.Host;
import Opal;

using namespace Opal;

#include "Build/RecipeBuildRunner.h"
#include "LocalUserConfig/LocalUserConfigExtensions.h"
#include "Package/PackageManager.h"