#pragma once
#include "../Cache/UndocumentedApi.h"

namespace Monitor::Windows::Functions::Overrides::UndocumentedApi
{
	int WINAPI EntryPoint(void)
	{
		return Cache::UndocumentedApi::EntryPoint();
	}

	BOOL WINAPI PrivCopyFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::PrivCopyFileExA));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		bool result = 0;
		if (blockAccess)
		{
			result = FALSE;
			SetLastError(ERROR_ACCESS_DENIED);
		}
		else
		{
			result = Cache::UndocumentedApi::PrivCopyFileExA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::PrivCopyFileExW));

		// Check if this file is allowed access
		bool blockReadAccess = !FileSystemAccessSandbox::IsReadAllowed(lpExistingFileName);
		bool blockWriteAccess = !FileSystemAccessSandbox::IsWriteAllowed(lpNewFileName);
		bool blockAccess = blockReadAccess || blockWriteAccess;
		bool result = 0;
		if (blockAccess)
		{
			result = FALSE;
			SetLastError(ERROR_ACCESS_DENIED);
		}
		else
		{
			result = Cache::UndocumentedApi::PrivCopyFileExW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}

		message.AppendValue(lpExistingFileName);
		message.AppendValue(lpNewFileName);
		message.AppendValue(result);
		message.AppendValue(blockAccess);

		return result;
	}
}