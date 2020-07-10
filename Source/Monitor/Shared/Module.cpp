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

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

export module Monitor.Shared;
import Detours;
import Opal;

using namespace Opal;

export constexpr const char* TBLOG_PIPE_NAMEA = "\\\\.\\pipe\\tracebuild";
export constexpr const wchar_t* TBLOG_PIPE_NAMEW = L"\\\\.\\pipe\\tracebuild";
#ifdef UNICODE
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEW;
#else
export constexpr const char* TBLOG_PIPE_NAME = TBLOG_PIPE_NAMEA;
#endif

namespace Monitor
{
	export enum class DetourMessageType : uint64_t
	{
		Exit,
		Error,
		CopyFile,
		CreateDirectory,
		CreateFile,
		CreateHardLink,
		CreateProcess,
		DeleteFile,
		GetEnvironmentVariable,
		GetFileAttributes,
		LoadLibrary,
		MoveFile,
		OpenFile,
	};

	export struct DetourMessage
	{
		DetourMessageType Type;
		uint64_t ContentSize;
		byte Content[2048 - sizeof(Type) - sizeof(ContentSize)];
	};

	export struct DetourPayload
	{
		DWORD nParentProcessId;
		DWORD nTraceProcessId;
		DWORD nGeneology;
		DWORD rGeneology[64];
		WCHAR wzParents[256];
	};

	// Shared state payload guid.
	//
	export constexpr GUID GuidTrace = { 0xd8e2dc69, 0x3004, 0x453e, { 0x94, 0x15, 0x19, 0x0e, 0x79, 0xe8, 0x93, 0x52 } };
}

export void ThrowIfFailed(int32_t result, std::string_view message)
{
	if (result != NO_ERROR)
	{
		throw std::runtime_error(message.data());
	}
}

#include "MockDetourProcessManager.h"
#include "WindowsDetourProcessManager.h"