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
		// Check if this file is allowed access
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		bool result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::PrivCopyFileExA(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::PrivCopyFileExA));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
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
		// Check if this file is allowed access
		bool blockReadAccess = !Monitor::FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !Monitor::FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		bool result = 0;
		__try
		{
			if (blockAccess)
			{
				result = FALSE;
				SetLastError(ERROR_ACCESS_DENIED);
			}
			else
			{
				result = Cache::PrivCopyFileExW(
					lpExistingFileName,
					lpNewFileName,
					lpProgressRoutine,
					lpData,
					pbCancel,
					dwCopyFlags);
			}
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::PrivCopyFileExW));
			message.AppendValue(lpExistingFileName);
			message.AppendValue(lpNewFileName);
			message.AppendValue(result);
			message.AppendValue(blockAccess);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}
}