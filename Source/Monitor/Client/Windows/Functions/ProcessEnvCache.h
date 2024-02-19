#pragma once

namespace Functions::ProcessEnv::Cache
{
	/// <summary>
	/// Searches for a specified file in a specified path.
	/// </summary>
	DWORD (WINAPI* SearchPathA)(
		LPCSTR lpPath,
		LPCSTR lpFileName,
		LPCSTR lpExtension,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart) = ::SearchPathA;
	DWORD (WINAPI* SearchPathW)(
		LPCWSTR lpPath,
		LPCWSTR lpFileName,
		LPCWSTR lpExtension,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart) = ::SearchPathW;
}