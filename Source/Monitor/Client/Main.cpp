
// TODO: Warning unsafe method
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <algorithm>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

#ifdef _WIN32

#include <windows.h>
#include <wofapi.h>

#ifdef CreateProcess
#undef CreateProcess
#endif

#include <stdio.h>
#include <strsafe.h>

#define DllExport __declspec(dllexport)

#elif defined(__linux__)

#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>

#endif

import Opal;
import Monitor.Shared;

#ifdef _WIN32

import Detours;

#include "Windows/ConnectionManager.h"
#include "Windows/DllMain.h"

#elif defined(__linux__)

#include "Linux/ConnectionManager.h"
#include "Linux/Startup.h"

#endif