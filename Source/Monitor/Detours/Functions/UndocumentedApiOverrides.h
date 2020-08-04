#pragma once
#include "UndocumentedApiCache.h"

namespace Functions::UndocumentedApi::Overrides
{
	int WINAPI EntryPoint(void)
	{
		return Cache::EntryPoint();
	}

	BOOL WINAPI PrivCopyFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		bool result = 0;
		__try
		{
			result = Cache::PrivCopyFileExA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::PrivCopyFileExA;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI PrivCopyFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		bool result = 0;
		__try
		{
			result = Cache::PrivCopyFileExW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::PrivCopyFileExW;
			EventLogger::AppendValue(message, lpExistingFileName);
			EventLogger::AppendValue(message, lpNewFileName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
}