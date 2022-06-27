module;

#ifdef WIN32
#include <windows.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103)
#endif
#include <strsafe.h>
#pragma warning(pop)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <atomic>
#include <array>
#include <codecvt>
#include <iostream>
#include <locale>
#include <map>
#include <memory>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

export module Monitor.Host;
#ifdef WIN32
import Detours;
#endif
import Monitor.Shared;
import Opal;

using namespace Opal;

#include "MockDetourProcessManager.h"
#ifdef WIN32
#include "WindowsDetourProcessManager.h"
#endif
#include "ScopedDetourProcessManagerRegister.h"

#include "DetourCallbackLogger.h"
#include "ForkCallbackLogger.h"