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

export module Opal;

import HttpLib;

#include "Core/Path.h"
#include "Core/SemanticVersion.h"

#include "IO/SystemConsoleManager.h"
#include "IO/MockConsoleManager.h"
#include "IO/ScopedConsoleManagerRegister.h"

#include "Logger/Log.h"
#include "Logger/ConsoleTraceListener.h"
#include "Logger/ScopedTraceListenerRegister.h"
#include "Logger/TestTraceListener.h"

#include "Memory/IReferenceCounted.h"
#include "Memory/Reference.h"
#include "Memory/ReferenceCounted.h"

#include "Network/HttpLibNetworkManager.h"
#include "Network/MockNetworkManager.h"
#include "Network/ScopedNetworkManagerRegister.h"

#include "System/SmartHandle.h"
#include "System/DynamicLibraryManager.h"
#include "System/ProcessManager.h"
#include "System/MockEnvironment.h"
#include "System/MockFileSystem.h"
#include "System/MockProcessManager.h"
#include "System/PlatformProcessManager.h"
#include "System/ScopedFileSystemRegister.h"
#include "System/ScopedProcessManagerRegister.h"
#include "System/STLEnvironment.h"
#include "System/STLFileSystem.h"
