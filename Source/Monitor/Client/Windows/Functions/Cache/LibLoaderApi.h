#pragma once

namespace Monitor::Windows::Functions::Cache::LibLoaderApi
{
	/// <summary>
	/// Loads the specified module into the address space of the calling process.
	/// </summary>
	HMODULE (WINAPI* LoadLibraryA)(
		LPCSTR lpLibFileName) = ::LoadLibraryA;
	HMODULE (WINAPI* LoadLibraryW)(
		LPCWSTR lpLibFileName) = ::LoadLibraryW;

	/// <summary>
	/// Loads the specified module into the address space of the calling process.
	/// </summary>
	HMODULE (WINAPI* LoadLibraryExA)(
		LPCSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags) = ::LoadLibraryExA;
	HMODULE (WINAPI* LoadLibraryExW)(
		LPCWSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags) = ::LoadLibraryExW;
}