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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadLibraryA));
			message.AppendValue(lpLibFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadLibraryW));
			message.AppendValue(lpLibFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadLibraryExA));
			message.AppendValue(lpLibFileName);
			Monitor::ConnectionManager::WriteMessage(message);
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
			auto message = Monitor::Message();
			message.Type = Monitor::MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(Monitor::DetourEventType::LoadLibraryExW));
			message.AppendValue(lpLibFileName);
			Monitor::ConnectionManager::WriteMessage(message);
		}

		return result;
	}
}