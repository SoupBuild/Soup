module;

#ifdef _WIN32

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
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

export module Monitor.Shared;

export constexpr const char* TBLOG_PIPE_NAMEA = "\\\\.\\pipe\\monitor";
export constexpr const wchar_t* TBLOG_PIPE_NAMEW = L"\\\\.\\pipe\\monitor";
#ifdef UNICODE
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEW;
#else
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEA;
#endif

#include "DetourEventType.h"
#include "Message.h"
#include "ProcessPayload.h"

#ifdef _WIN32

export void ThrowIfFailed(int32_t result, std::string_view message)
{
	if (result != NO_ERROR)
	{
		throw std::runtime_error(message.data());
	}
}

#endif