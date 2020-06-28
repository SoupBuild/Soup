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

export struct TBLOG_MESSAGE
{
	DWORD nBytes;
	CHAR szMessage[32764]; // 32768 - sizeof(nBytes)
};

export struct TBLOG_PAYLOAD
{
	DWORD nParentProcessId;
	DWORD nTraceProcessId;
	DWORD nGeneology;
	DWORD rGeneology[64];
	WCHAR wzParents[256];
	WCHAR wzzDrop[1024];  // Like an environment: zero terminated strings with a last zero.
	WCHAR wzzEnvironment[32768];
};

// Shared state payload guid.
//
export constexpr GUID s_guidTrace = { 0xd8e2dc69, 0x3004, 0x453e, { 0x94, 0x15, 0x19, 0x0e, 0x79, 0xe8, 0x93, 0x52 } };

export void ThrowIfFailed(int32_t result, std::string_view message)
{
	if (result != NO_ERROR)
	{
		throw std::runtime_error(message.data());
	}
}
