#pragma once
#include "../Cache/LibLoaderApi.h"
#include "Windows/ConnectionManager.h"

namespace Monitor::Windows::Functions::Overrides::LibLoaderApi
{
	HMODULE WINAPI LoadLibraryA(
		LPCSTR lpLibFileName)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LibLoaderApi::LoadLibraryA(
				lpLibFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryA));
			message.AppendValue(lpLibFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}

	HMODULE WINAPI LoadLibraryW(
		LPCWSTR lpLibFileName)
	{
		HMODULE result = 0;
		__try
		{
			result = Cache::LibLoaderApi::LoadLibraryW(
				lpLibFileName);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryW));
			message.AppendValue(lpLibFileName);
			connectionManager.WriteMessage(message);
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
			result = Cache::LibLoaderApi::LoadLibraryExA(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryExA));
			message.AppendValue(lpLibFileName);
			connectionManager.WriteMessage(message);
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
			result = Cache::LibLoaderApi::LoadLibraryExW(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			auto message = Message();
			message.Type = MessageType::Detour;
			message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryExW));
			message.AppendValue(lpLibFileName);
			connectionManager.WriteMessage(message);
		}

		return result;
	}
}