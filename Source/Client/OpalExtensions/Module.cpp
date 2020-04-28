module;
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <locale>
#include <map>
#include <queue>
#include <sstream>
#include <string>

#include <Windows.h>
#include <shlobj.h>
#include <psapi.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

export module Opal.Extensions;

import HttpLib;

#include "Network/HttpLibNetworkManager.h"
#include "Network/MockNetworkManager.h"
#include "Network/ScopedNetworkManagerRegister.h"
