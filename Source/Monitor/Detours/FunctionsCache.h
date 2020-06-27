#pragma once

namespace Functions::Cache
{
	int (WINAPI* EntryPoint)() = nullptr;

	BOOL (WINAPI* CreateDirectoryW)(
		LPCWSTR,
		LPSECURITY_ATTRIBUTES) = ::CreateDirectoryW;

	BOOL (WINAPI* CreateDirectoryExW)(
		LPCWSTR,
		LPCWSTR,
		LPSECURITY_ATTRIBUTES) = ::CreateDirectoryExW;

	HANDLE (WINAPI* CreateFileA)(
		LPCSTR,
		DWORD,
		DWORD,
		LPSECURITY_ATTRIBUTES,
		DWORD,
		DWORD,
		HANDLE) = ::CreateFileA;

	HANDLE (WINAPI* CreateFileW)(
		LPCWSTR,
		DWORD,
		DWORD,
		LPSECURITY_ATTRIBUTES,
		DWORD,
		DWORD,
		HANDLE) = ::CreateFileW;

	HANDLE (WINAPI* CreateFileMappingW)(
		HANDLE,
		LPSECURITY_ATTRIBUTES,
		DWORD,
		DWORD,
		DWORD,
		LPCWSTR) = ::CreateFileMappingW;

	BOOL (WINAPI* CreatePipe)(
		PHANDLE,
		PHANDLE,
		LPSECURITY_ATTRIBUTES,
		DWORD) = ::CreatePipe;

	BOOL (WINAPI* CloseHandle)(
		HANDLE) = ::CloseHandle;

	BOOL (WINAPI* DuplicateHandle)(
		HANDLE,
		HANDLE,
		HANDLE,
		LPHANDLE,
		DWORD,
		BOOL,
		DWORD) = ::DuplicateHandle;

	BOOL (WINAPI* CreateProcessW)(
		LPCWSTR,
		LPWSTR,
		LPSECURITY_ATTRIBUTES,
		LPSECURITY_ATTRIBUTES,
		BOOL,
		DWORD,
		LPVOID,
		LPCWSTR,
		LPSTARTUPINFOW,
		LPPROCESS_INFORMATION) = ::CreateProcessW;

	BOOL (WINAPI* CreateProcessA)(
		LPCSTR,
		LPSTR,
		LPSECURITY_ATTRIBUTES,
		LPSECURITY_ATTRIBUTES,
		BOOL,
		DWORD,
		LPVOID,
		LPCSTR,
		LPSTARTUPINFOA,
		LPPROCESS_INFORMATION) = ::CreateProcessA;

	BOOL (WINAPI* DeleteFileW)(
		LPCWSTR) = ::DeleteFileW;

	BOOL (WINAPI* DeviceIoControl)(
		HANDLE,
		DWORD,
		LPVOID,
		DWORD,
		LPVOID,
		DWORD,
		LPDWORD,
		LPOVERLAPPED) = ::DeviceIoControl;

	DWORD (WINAPI* GetFileAttributesW)(
		LPCWSTR) = ::GetFileAttributesW;

	BOOL (WINAPI* MoveFileWithProgressW)(
		LPCWSTR,
		LPCWSTR,
		LPPROGRESS_ROUTINE,
		LPVOID,
		DWORD) = ::MoveFileWithProgressW;

	BOOL (WINAPI* MoveFileA)(
		LPCSTR,
		LPCSTR) = ::MoveFileA;

	BOOL (WINAPI* MoveFileW)(
		LPCWSTR,
		LPCWSTR) = ::MoveFileW;

	BOOL (WINAPI* MoveFileExA)(
		LPCSTR,
		LPCSTR,
		DWORD) = ::MoveFileExA;

	BOOL (WINAPI* MoveFileExW)(
		LPCWSTR,
		LPCWSTR,
		DWORD) = ::MoveFileExW;

	BOOL (WINAPI* CopyFileExA)(
		LPCSTR,
		LPCSTR,
		LPPROGRESS_ROUTINE,
		LPVOID,
		LPBOOL,
		DWORD) = ::CopyFileExA;

	BOOL (WINAPI* CopyFileExW)(
		LPCWSTR,
		LPCWSTR,
		LPPROGRESS_ROUTINE,
		LPVOID,
		LPBOOL,
		DWORD) = ::CopyFileExW;

	BOOL (WINAPI* PrivCopyFileExW)(
		LPCWSTR,
		LPCWSTR,
		LPPROGRESS_ROUTINE,
		LPVOID,
		LPBOOL,
		DWORD) = nullptr;

	BOOL (WINAPI* CreateHardLinkA)(
		LPCSTR,
		LPCSTR,
		LPSECURITY_ATTRIBUTES) = ::CreateHardLinkA;

	BOOL (WINAPI* CreateHardLinkW)(
		LPCWSTR,
		LPCWSTR,
		LPSECURITY_ATTRIBUTES) = ::CreateHardLinkW;

	BOOL (WINAPI* SetStdHandle)(
		DWORD,
		HANDLE) = ::SetStdHandle;

	HMODULE (WINAPI* LoadLibraryA)(
		LPCSTR) = ::LoadLibraryA;

	HMODULE (WINAPI* LoadLibraryW)(
		LPCWSTR) = ::LoadLibraryW;

	HMODULE (WINAPI* LoadLibraryExA)(
		LPCSTR,
		HANDLE,
		DWORD) = ::LoadLibraryExA;

	HMODULE (WINAPI* LoadLibraryExW)(
		LPCWSTR,
		HANDLE,
		DWORD) = ::LoadLibraryExW;

	DWORD (WINAPI* SetFilePointer)(
		HANDLE,
		LONG,
		PLONG,
		DWORD) = ::SetFilePointer;

	BOOL (WINAPI* SetFilePointerEx)(
		HANDLE,
		LARGE_INTEGER,
		PLARGE_INTEGER,
		DWORD) = ::SetFilePointerEx;

	BOOL (WINAPI* ReadFile)(
		HANDLE,
		LPVOID,
		DWORD,
		LPDWORD,
		LPOVERLAPPED) = ::ReadFile;

	BOOL (WINAPI* ReadFileEx)(
		HANDLE,
		LPVOID,
		DWORD,
		LPOVERLAPPED,
		LPOVERLAPPED_COMPLETION_ROUTINE) = ::ReadFileEx;

	BOOL (WINAPI* WriteFile)(
		HANDLE,
		LPCVOID,
		DWORD,
		LPDWORD,
		LPOVERLAPPED) = ::WriteFile;

	BOOL (WINAPI* WriteFileEx)(
		HANDLE,
		LPCVOID,
		DWORD,
		LPOVERLAPPED,
		LPOVERLAPPED_COMPLETION_ROUTINE) = ::WriteFileEx;

	BOOL (WINAPI* WriteConsoleA)(
		HANDLE,
		const void*,
		DWORD,
		LPDWORD,
		LPVOID) = ::WriteConsoleA;

	BOOL (WINAPI* WriteConsoleW)(
		HANDLE,
		const void*,
		DWORD,
		LPDWORD,
		LPVOID) = ::WriteConsoleW;

	void (WINAPI* ExitProcess)(
		UINT) = ::ExitProcess;

	DWORD (WINAPI* ExpandEnvironmentStringsA)(
		PCSTR,
		PCHAR,
		DWORD) = ::ExpandEnvironmentStringsA;

	DWORD (WINAPI* ExpandEnvironmentStringsW)(
		PCWSTR,
		PWCHAR,
		DWORD) = ::ExpandEnvironmentStringsW;

	DWORD (WINAPI* GetEnvironmentVariableA)(
		PCSTR,
		PCHAR,
		DWORD) = ::GetEnvironmentVariableA;

	DWORD (WINAPI* GetEnvironmentVariableW)(
		PCWSTR,
		PWCHAR,
		DWORD) = ::GetEnvironmentVariableW;
}