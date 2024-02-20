#pragma once
#include "ProcessEnvCache.h"
#include "WindowsConnectionManager.h"

namespace Functions::ProcessEnv::Overrides
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
			result = Cache::SearchPathA(
				lpPath,
				lpFileName,
				lpExtension,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SearchPathA));
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
			result = Cache::SearchPathW(
				lpPath,
				lpFileName,
				lpExtension,
				nBufferLength,
				lpBuffer,
				lpFilePart);
		}
		__finally
		{
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::SearchPathW));
			message.AppendValue(lpPath);
			message.AppendValue(lpFileName);
			message.AppendValue(lpExtension);
			message.AppendValue(result);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}