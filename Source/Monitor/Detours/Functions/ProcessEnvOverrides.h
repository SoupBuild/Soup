#pragma once
#include "ProcessEnvCache.h"

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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SearchPathA;
			EventLogger::AppendValue(message, lpPath);
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExtension);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
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
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::SearchPathW;
			EventLogger::AppendValue(message, lpPath);
			EventLogger::AppendValue(message, lpFileName);
			EventLogger::AppendValue(message, lpExtension);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
}