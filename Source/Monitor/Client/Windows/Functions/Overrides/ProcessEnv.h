#pragma once
#include "../Cache/ProcessEnv.h"
#include "Windows/ConnectionManager.h"

namespace Monitor::Windows::Functions::Overrides::ProcessEnv
{
	DWORD WINAPI SearchPathA(
		LPCSTR lpPath,
		LPCSTR lpFileName,
		LPCSTR lpExtension,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::ProcessEnv::SearchPathA(
				lpPath,
				lpFileName,
				lpExtension,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SearchPathA));
			message.AppendValue(lpPath);
			message.AppendValue(lpFileName);
			message.AppendValue(lpExtension);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	DWORD WINAPI SearchPathW(
		LPCWSTR lpPath,
		LPCWSTR lpFileName,
		LPCWSTR lpExtension,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart)
	{
		DWORD result = 0;
		__try
		{
			result = Cache::ProcessEnv::SearchPathW(
				lpPath,
				lpFileName,
				lpExtension,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::SearchPathW));
			message.AppendValue(lpPath);
			message.AppendValue(lpFileName);
			message.AppendValue(lpExtension);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}