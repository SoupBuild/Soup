#pragma once
#include "../Cache/LibLoaderApi.h"
#include "Windows/ConnectionManager.h"

namespace Monitor::Windows::Functions::Overrides::LibLoaderApi
{
	HMODULE WINAPI LoadLibraryA(
		LPCSTR lpLibFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryA));

		HMODULE result = Cache::LibLoaderApi::LoadLibraryA(lpLibFileName);

		message.AppendValue(lpLibFileName);

		return result;
	}

	HMODULE WINAPI LoadLibraryW(
		LPCWSTR lpLibFileName)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryW));

		HMODULE result = Cache::LibLoaderApi::LoadLibraryW(lpLibFileName);

		message.AppendValue(lpLibFileName);

		return result;
	}

	HMODULE WINAPI LoadLibraryExA(
		LPCSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryExA));

		HMODULE result = Cache::LibLoaderApi::LoadLibraryExA(
			lpLibFileName,
			hFile,
			dwFlags);

		message.AppendValue(lpLibFileName);

		return result;
	}

	HMODULE WINAPI LoadLibraryExW(
		LPCWSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::LoadLibraryExW));

		HMODULE result = Cache::LibLoaderApi::LoadLibraryExW(
			lpLibFileName,
			hFile,
			dwFlags);

		message.AppendValue(lpLibFileName);

		return result;
	}
}