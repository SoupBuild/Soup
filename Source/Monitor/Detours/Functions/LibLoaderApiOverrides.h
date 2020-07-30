#pragma once
#include "LibLoaderApiCache.h"

namespace Functions::LibLoaderApi::Overrides
{
	HMODULE WINAPI LoadLibraryA(
		LPCSTR lpLibFileName)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LoadLibraryA(
				lpLibFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadLibraryA;
			EventLogger::AppendValue(message, lpLibFileName);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	HMODULE WINAPI LoadLibraryW(
		LPCWSTR lpLibFileName)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LoadLibraryW(
				lpLibFileName);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadLibraryW;
			EventLogger::AppendValue(message, lpLibFileName);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	HMODULE WINAPI LoadLibraryExA(
		LPCSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LoadLibraryExA(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadLibraryExA;
			EventLogger::AppendValue(message, lpLibFileName);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	HMODULE WINAPI LoadLibraryExW(
		LPCWSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LoadLibraryExW(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::LoadLibraryExW;
			EventLogger::AppendValue(message, lpLibFileName);
			EventLogger::WriteMessage(message);
		}

		return result;
	}
}