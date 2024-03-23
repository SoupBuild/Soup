#pragma once

namespace Monitor::Windows::Functions::Cache::UndocumentedApi
{
	/// <summary>
	/// Entry Point
	/// </summary>
	int (WINAPI* EntryPoint)() = nullptr;

	/// <summary>
	/// Undocumented api for copy
	/// Note: Used in xcopy
	/// </summary>
	BOOL (WINAPI* PrivCopyFileExA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags) = nullptr;
	BOOL (WINAPI* PrivCopyFileExW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags) = nullptr;
}