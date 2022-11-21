module;

#include <windows.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103)
#endif
#include <strsafe.h>
#pragma warning(pop)

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <atomic>
#include <array>
#include <codecvt>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

export module Monitor.Host;
import Detours;
import Monitor.Shared;
import Opal;

using namespace Opal;

#include "MockDetourProcessManager.h"
#include "WindowsDetourProcessManager.h"
#include "ScopedDetourProcessManagerRegister.h"

#include "DetourCallbackLogger.h"
#include "ForkCallbackLogger.h"