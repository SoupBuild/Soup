module;
#include <cstdint>
#include <stdarg.h>
#include <stdexcept>
#include <string_view>
#include <Windows.h>

export module Monitor.Shared;

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
