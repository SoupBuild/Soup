#pragma once

namespace Functions::Cache
{
	int (WINAPI* EntryPoint)() = nullptr;

	BOOL (WINAPI* CreateDirectoryW)(
		LPCWSTR a0,
		LPSECURITY_ATTRIBUTES a1) = ::CreateDirectoryW;

	BOOL (WINAPI* CreateDirectoryExW)(
		LPCWSTR a0,
		LPCWSTR a1,
		LPSECURITY_ATTRIBUTES a2) = ::CreateDirectoryExW;

	HANDLE (WINAPI* CreateFileW)(
		LPCWSTR a0,
		DWORD a1,
		DWORD a2,
		LPSECURITY_ATTRIBUTES a3,
		DWORD a4,
		DWORD a5,
		HANDLE a6) = ::CreateFileW;

	HANDLE (WINAPI* CreateFileMappingW)(
		HANDLE hFile,
		LPSECURITY_ATTRIBUTES lpAttributes,
		DWORD flProtect,
		DWORD dwMaximumSizeHigh,
		DWORD dwMaximumSizeLow,
		LPCWSTR lpName) = ::CreateFileMappingW;

	BOOL (WINAPI* CreatePipe)(
		PHANDLE hReadPipe,
		PHANDLE hWritePipe,
		LPSECURITY_ATTRIBUTES lpPipeAttributes,
		DWORD nSize) = ::CreatePipe;

	BOOL (WINAPI* CloseHandle)(
		HANDLE a0) = ::CloseHandle;

	BOOL (WINAPI* DuplicateHandle)(
		HANDLE hSourceProcessHandle,
		HANDLE hSourceHandle,
		HANDLE hTargetProcessHandle,
		LPHANDLE lpTargetHandle,
		DWORD dwDesiredAccess,
		BOOL bInheritHandle,
		DWORD dwOptions) = ::DuplicateHandle;

	BOOL (WINAPI* CreateProcessW)(
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessW;

	BOOL (WINAPI* CreateProcessA)(
		LPCSTR lpApplicationName,
		LPSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCSTR lpCurrentDirectory,
		LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessA;

	BOOL (WINAPI* DeleteFileW)(
		LPCWSTR a0) = ::DeleteFileW;

	BOOL (WINAPI* DeviceIoControl)(
		HANDLE a0,
		DWORD dwIoControlCode,
		LPVOID lpInBuffer,
		DWORD nInBufferSize,
		LPVOID lpOutBuffer,
		DWORD nOutBufferSize,
		LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped) = ::DeviceIoControl;

	DWORD (WINAPI* GetFileAttributesW)(
		LPCWSTR a0) = ::GetFileAttributesW;

	BOOL (WINAPI* MoveFileWithProgressW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags) = ::MoveFileWithProgressW;

	BOOL (WINAPI* MoveFileA)(
		LPCSTR a0,
		LPCSTR a1) = ::MoveFileA;

	BOOL (WINAPI* MoveFileW)(
		LPCWSTR a0,
		LPCWSTR a12) = ::MoveFileW;

	BOOL (WINAPI* MoveFileExA)(
		LPCSTR a0,
		LPCSTR a1,
		DWORD a2) = ::MoveFileExA;

	BOOL (WINAPI* MoveFileExW)(
		LPCWSTR a0,
		LPCWSTR a1,
		DWORD a2) = ::MoveFileExW;

	BOOL (WINAPI* CopyFileExA)(
		LPCSTR a0,
		LPCSTR a1,
		LPPROGRESS_ROUTINE a2,
		LPVOID a4,
		LPBOOL a5,
		DWORD a6) = ::CopyFileExA;

	BOOL (WINAPI* CopyFileExW)(
		LPCWSTR a0,
		LPCWSTR a1,
		LPPROGRESS_ROUTINE a2,
		LPVOID a4,
		LPBOOL a5,
		DWORD a6) = ::CopyFileExW;

	BOOL (WINAPI* PrivCopyFileExW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD  dwCopyFlags) = nullptr;

	BOOL (WINAPI* CreateHardLinkA)(
		LPCSTR a0,
		LPCSTR a1,
		LPSECURITY_ATTRIBUTES a2) = ::CreateHardLinkA;

	BOOL (WINAPI* CreateHardLinkW)(
		LPCWSTR a0,
		LPCWSTR a1,
		LPSECURITY_ATTRIBUTES a2) = ::CreateHardLinkW;

	BOOL (WINAPI* SetStdHandle)(
		DWORD a0,
		HANDLE a1) = ::SetStdHandle;

	HMODULE (WINAPI* LoadLibraryA)(
		LPCSTR a0) = ::LoadLibraryA;

	HMODULE (WINAPI* LoadLibraryW)(
		LPCWSTR a0) = ::LoadLibraryW;

	HMODULE (WINAPI* LoadLibraryExA)(
		LPCSTR a0,
		HANDLE a1,
		DWORD a2) = ::LoadLibraryExA;

	HMODULE (WINAPI* LoadLibraryExW)(
		LPCWSTR a0,
		HANDLE a1,
		DWORD a2) = ::LoadLibraryExW;

	DWORD (WINAPI* SetFilePointer)(
		HANDLE hFile,
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh,
		DWORD dwMoveMethod) = ::SetFilePointer;

	BOOL (WINAPI* SetFilePointerEx)(
		HANDLE hFile,
		LARGE_INTEGER liDistanceToMove,
		PLARGE_INTEGER lpNewFilePointer,
		DWORD dwMoveMethod) = ::SetFilePointerEx;

	BOOL (WINAPI* ReadFile)(
		HANDLE a0,
		LPVOID a1,
		DWORD a2,
		LPDWORD a3,
		LPOVERLAPPED a4) = ::ReadFile;

	BOOL (WINAPI* ReadFileEx)(
		HANDLE a0,
		LPVOID a1,
		DWORD a2,
		LPOVERLAPPED a3,
		LPOVERLAPPED_COMPLETION_ROUTINE a4) = ::ReadFileEx;

	BOOL (WINAPI* WriteFile)(
		HANDLE a0,
		LPCVOID a1,
		DWORD a2,
		LPDWORD a3,
		LPOVERLAPPED a4) = ::WriteFile;

	BOOL (WINAPI* WriteFileEx)(
		HANDLE a0,
		LPCVOID a1,
		DWORD a2,
		LPOVERLAPPED a3,
		LPOVERLAPPED_COMPLETION_ROUTINE a4) = ::WriteFileEx;

	BOOL (WINAPI* WriteConsoleA)(
		HANDLE a0,
		const void* a1,
		DWORD a2,
		LPDWORD a3,
		LPVOID a4) = ::WriteConsoleA;

	BOOL (WINAPI* WriteConsoleW)(
		HANDLE a0,
		const void* a1,
		DWORD a2,
		LPDWORD a3,
		LPVOID a4) = ::WriteConsoleW;

	void (WINAPI* ExitProcess)(
		UINT a0) = ::ExitProcess;

	DWORD (WINAPI* ExpandEnvironmentStringsA)(
		PCSTR lpSrc,
		PCHAR lpDst,
		DWORD nSize) = ::ExpandEnvironmentStringsA;

	DWORD (WINAPI* ExpandEnvironmentStringsW)(
		PCWSTR lpSrc,
		PWCHAR lpDst,
		DWORD nSize) = ::ExpandEnvironmentStringsW;

	DWORD (WINAPI* GetEnvironmentVariableA)(
		PCSTR lpName,
		PCHAR lpBuffer,
		DWORD nSize) = ::GetEnvironmentVariableA;

	DWORD (WINAPI* GetEnvironmentVariableW)(
		PCWSTR lpName,
		PWCHAR lpBuffer,
		DWORD nSize) = ::GetEnvironmentVariableW;

	PCWSTR (CDECL* wgetenv)(PCWSTR var) = nullptr;
	PCSTR (CDECL* getenv)(PCSTR var) = nullptr;
	DWORD (CDECL* getenv_s)(DWORD *pValue, PCHAR pBuffer, DWORD cBuffer, PCSTR varname) = nullptr;
	DWORD (CDECL* wgetenv_s)(DWORD *pValue, PWCHAR pBuffer, DWORD cBuffer, PCWSTR varname) = nullptr;
	DWORD (CDECL* dupenv_s)(PCHAR *ppBuffer, DWORD *pcBuffer, PCSTR varname) = nullptr;
	DWORD (CDECL* wdupenv_s)(PWCHAR *ppBuffer, DWORD *pcBuffer, PCWSTR varname) = nullptr;
}