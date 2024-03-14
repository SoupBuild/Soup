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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SearchPathA));

		DWORD result = Cache::ProcessEnv::SearchPathA(
			lpPath,
			lpFileName,
			lpExtension,
			nBufferLength,
			lpBuffer,
			lpFilePart);

		message.AppendValue(lpPath);
		message.AppendValue(lpFileName);
		message.AppendValue(lpExtension);
		message.AppendValue(result);

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
		auto message = MessageSender(MessageType::Detour);
		message.AppendValue(static_cast<uint32_t>(DetourEventType::SearchPathW));

		DWORD result = Cache::ProcessEnv::SearchPathW(
			lpPath,
			lpFileName,
			lpExtension,
			nBufferLength,
			lpBuffer,
			lpFilePart);

		message.AppendValue(lpPath);
		message.AppendValue(lpFileName);
		message.AppendValue(lpExtension);
		message.AppendValue(result);

		return result;
	}
}