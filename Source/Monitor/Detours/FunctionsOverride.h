#pragma once
#include "FunctionsCache.h"
#include "Helpers.h"
#include "LoggingSystem.h"

namespace Functions::Override
{
	BOOL WINAPI CreateProcessW(
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		if (lpCommandLine == nullptr)
		{
			lpCommandLine = (LPWSTR)lpApplicationName;
		}

		bool rv = 0;
		__try
		{
			LPPROCESS_INFORMATION ppi = lpProcessInformation;
			PROCESS_INFORMATION pi;
			if (ppi == nullptr)
			{
				ppi = &pi;
			}

			rv = DetourCreateProcessWithDllExW(
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags | CREATE_SUSPENDED,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				ppi,
				s_szDllPath,
				Functions::Cache::CreateProcessW);

			if (rv)
			{
				CreateProcessInternals(
					ppi->hProcess);

				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(ppi->hThread);
				}

				if (ppi == &pi)
				{
					CloseHandle(ppi->hThread);
					CloseHandle(ppi->hProcess);
				}
			}
		}
		__finally
		{
			s_eventLogger.LogCreateProcess(
				lpApplicationName);
		}

		return rv;
	}

	BOOL WINAPI CreateProcessA(
		LPCSTR lpApplicationName,
		LPSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCSTR lpCurrentDirectory,
		LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		if (lpCommandLine == nullptr)
		{
			lpCommandLine = (LPSTR)lpApplicationName;
		}

		bool rv = 0;
		__try
		{
			LPPROCESS_INFORMATION ppi = lpProcessInformation;
			PROCESS_INFORMATION pi;
			if (ppi == nullptr)
			{
				ppi = &pi;
			}

			rv = DetourCreateProcessWithDllExA(
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags | CREATE_SUSPENDED,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				ppi,
				s_szDllPath,
				Functions::Cache::CreateProcessA);

			if (rv)
			{
				CreateProcessInternals(
					ppi->hProcess);

				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(ppi->hThread);
				}

				if (ppi == &pi)
				{
					CloseHandle(ppi->hThread);
					CloseHandle(ppi->hProcess);
				}
			}
		}
		__finally
		{
			s_eventLogger.LogCreateProcess(
				lpApplicationName);
		}

		return rv;
	}

	BOOL WINAPI CopyFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CopyFileExA(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			s_eventLogger.LogCopyFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI CopyFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CopyFileExW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			s_eventLogger.LogCopyFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI PrivCopyFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::PrivCopyFileExW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				pbCancel,
				dwCopyFlags);
		}
		__finally
		{
			s_eventLogger.LogCopyFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI CreateHardLinkA(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateHardLinkA(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateHardLink(
				lpFileName,
				lpExistingFileName);
		}

		return rv;
	}

	BOOL WINAPI CreateHardLinkW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateHardLinkW(
				lpFileName,
				lpExistingFileName,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateHardLink(
				lpFileName,
				lpExistingFileName);
		}

		return rv;
	}

	BOOL WINAPI CreateDirectoryA(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryA(
				lpPathName,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateDirectory(
				lpPathName);
		}

		return rv;
	}

	BOOL WINAPI CreateDirectoryExA(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryExA(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateDirectory(
				lpNewDirectory);
		}

		return rv;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryW(
				lpPathName,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateDirectory(
				lpPathName);
		}

		return rv;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryExW(
				lpTemplateDirectory,
				lpNewDirectory,
				lpSecurityAttributes);
		}
		__finally
		{
			s_eventLogger.LogCreateDirectory(
				lpNewDirectory);
		}

		return rv;
	}

	HANDLE WINAPI CreateFileA(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		HANDLE rv = 0;
		__try
		{
			rv = Functions::Cache::CreateFileA(
				lpFileName,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwCreationDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);
		}
		__finally
		{
			switch (dwCreationDisposition)
			{
				case CREATE_NEW:
				case CREATE_ALWAYS:
					s_eventLogger.LogCreateFile(lpFileName);
					break;
				case OPEN_EXISTING:
				case OPEN_ALWAYS:
				case TRUNCATE_EXISTING:
					s_eventLogger.LogOpenFile(lpFileName);
					break;
				default:
					s_eventLogger.LogError("Unknown dwCreationDisposition: CreateFileA");
					break;
			}
		}

		return rv;
	}

	HANDLE WINAPI CreateFileW(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile)
	{
		HANDLE rv = 0;
		__try
		{
			rv = Functions::Cache::CreateFileW(
				lpFileName,
				dwDesiredAccess,
				dwShareMode,
				lpSecurityAttributes,
				dwCreationDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);
		}
		__finally
		{
			switch (dwCreationDisposition)
			{
				case CREATE_NEW:
				case CREATE_ALWAYS:
					s_eventLogger.LogCreateFile(lpFileName);
					break;
				case OPEN_EXISTING:
				case OPEN_ALWAYS:
				case TRUNCATE_EXISTING:
					s_eventLogger.LogOpenFile(lpFileName);
					break;
				default:
					s_eventLogger.LogError("Unknown dwCreationDisposition: CreateFileW");
					break;
			}
		}

		return rv;
	}

	BOOL WINAPI DeleteFileA(
		LPCSTR lpFileName)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::DeleteFileA(lpFileName);
		}
		__finally
		{
			s_eventLogger.LogDeleteFile(lpFileName);
		}

		return rv;
	}

	BOOL WINAPI DeleteFileW(
		LPCWSTR lpFileName)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::DeleteFileW(lpFileName);
		}
		__finally
		{
			s_eventLogger.LogDeleteFile(lpFileName);
		}

		return rv;
	}

	DWORD WINAPI GetFileAttributesA(
		LPCSTR lpFileName)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::GetFileAttributesA(
				lpFileName);
		}
		__finally
		{
			s_eventLogger.LogGetFileAttributes(
				lpFileName);
		}

		return rv;
	}

	BOOL WINAPI GetFileAttributesExA(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		BOOL rv = 0;
		__try
		{
			rv = Functions::Cache::GetFileAttributesExA(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}
		__finally
		{
			s_eventLogger.LogGetFileAttributes(
				lpFileName);
		}

		return rv;
	}

	DWORD WINAPI GetFileAttributesW(
		LPCWSTR lpFileName)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::GetFileAttributesW(
				lpFileName);
		}
		__finally
		{
			s_eventLogger.LogGetFileAttributes(
				lpFileName);
		}

		return rv;
	}

	BOOL WINAPI GetFileAttributesExW(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation)
	{
		BOOL rv = 0;
		__try
		{
			rv = Functions::Cache::GetFileAttributesExW(
				lpFileName,
				fInfoLevelId,
				lpFileInformation);
		}
		__finally
		{
			s_eventLogger.LogGetFileAttributes(
				lpFileName);
		}

		return rv;
	}

	BOOL WINAPI MoveFileWithProgressW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileWithProgressW(
				lpExistingFileName,
				lpNewFileName,
				lpProgressRoutine,
				lpData,
				dwFlags);
		}
		__finally
		{
			s_eventLogger.LogMoveFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI MoveFileA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileA(
				lpExistingFileName,
				lpNewFileName);
		}
		__finally
		{
			s_eventLogger.LogMoveFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileW(
				lpExistingFileName,
				lpNewFileName);
		}
		__finally
		{
			s_eventLogger.LogMoveFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileExA(
				lpExistingFileName,
				lpNewFileName,
				dwFlags);
		}
		__finally
		{
			s_eventLogger.LogMoveFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags)
	{
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileExW(
				lpExistingFileName,
				lpNewFileName,
				dwFlags);
		}
		__finally
		{
			s_eventLogger.LogMoveFile(
				lpExistingFileName,
				lpNewFileName);
		}

		return rv;
	}

	HMODULE WINAPI LoadLibraryA(
		LPCSTR lpLibFileName)
	{
		HMODULE rv = 0;
		__try
		{
			rv = Functions::Cache::LoadLibraryA(
				lpLibFileName);
		}
		__finally
		{
			s_eventLogger.LogLoadLibrary(
				lpLibFileName);
		}

		return rv;
	}

	HMODULE WINAPI LoadLibraryW(
		LPCWSTR lpLibFileName)
	{
		HMODULE rv = 0;
		__try
		{
			rv = Functions::Cache::LoadLibraryW(
				lpLibFileName);
		}
		__finally
		{
			s_eventLogger.LogLoadLibrary(
				lpLibFileName);
		}

		return rv;
	}

	HMODULE WINAPI LoadLibraryExA(
		LPCSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		EnterFunc();

		HMODULE rv = 0;
		__try
		{
			rv = Functions::Cache::LoadLibraryExA(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			s_eventLogger.LogLoadLibrary(
				lpLibFileName);
		}

		return rv;
	}

	HMODULE WINAPI LoadLibraryExW(
		LPCWSTR lpLibFileName,
		HANDLE hFile,
		DWORD dwFlags)
	{
		HMODULE rv = 0;
		__try
		{
			rv = Functions::Cache::LoadLibraryExW(
				lpLibFileName,
				hFile,
				dwFlags);
		}
		__finally
		{
			// TODO: stuck
			//s_eventLogger.LogLoadLibrary(
			//	lpLibFileName);
		}

		return rv;
	}

	DWORD WINAPI SetFilePointer(
		HANDLE hFile,
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh,
		DWORD dwMoveMethod)
	{
		return Functions::Cache::SetFilePointer(
			hFile,
			lDistanceToMove,
			lpDistanceToMoveHigh,
			dwMoveMethod);
	}

	BOOL WINAPI SetFilePointerEx(
		HANDLE hFile,
		LARGE_INTEGER liDistanceToMove,
		PLARGE_INTEGER lpNewFilePointer,
		DWORD dwMoveMethod)
	{
		return Functions::Cache::SetFilePointerEx(
			hFile,
			liDistanceToMove,
			lpNewFilePointer,
			dwMoveMethod);
	}

	BOOL WINAPI ReadFile(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped)
	{
		return Functions::Cache::ReadFile(
			hFile,
			lpBuffer,
			nNumberOfBytesToRead,
			lpNumberOfBytesRead,
			lpOverlapped);
	}

	BOOL WINAPI ReadFileEx(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		return Functions::Cache::ReadFileEx(
			hFile,
			lpBuffer,
			nNumberOfBytesToRead,
			lpOverlapped,
			lpCompletionRoutine);
	}

	BOOL WINAPI WriteFile(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped)
	{
		return Functions::Cache::WriteFile(
			hFile,
			lpBuffer,
			nNumberOfBytesToWrite,
			lpNumberOfBytesWritten,
			lpOverlapped);
	}

	BOOL WINAPI WriteFileEx(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
	{
		return Functions::Cache::WriteFileEx(
			hFile,
			lpBuffer,
			nNumberOfBytesToWrite,
			lpOverlapped,
			lpCompletionRoutine);
	}

	BOOL WINAPI WriteConsoleA(
		HANDLE hConsoleOutput,
		const VOID *lpBuffer,
		DWORD nNumberOfCharsToWrite,
		LPDWORD lpNumberOfCharsWritten,
		LPVOID lpReserved)
	{
		return Functions::Cache::WriteConsoleA(
			hConsoleOutput,
			lpBuffer,
			nNumberOfCharsToWrite,
			lpNumberOfCharsWritten,
			lpReserved);
	}

	BOOL WINAPI WriteConsoleW(
		HANDLE hConsoleOutput,
		const VOID *lpBuffer,
		DWORD nNumberOfCharsToWrite,
		LPDWORD lpNumberOfCharsWritten,
		LPVOID lpReserved)
	{
		return Functions::Cache::WriteConsoleW(
			hConsoleOutput,
			lpBuffer,
			nNumberOfCharsToWrite,
			lpNumberOfCharsWritten,
			lpReserved);
	}

	DWORD WINAPI ExpandEnvironmentStringsA(
		PCSTR lpSrc,
		PCHAR lpDst,
		DWORD nSize)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::ExpandEnvironmentStringsA(
				lpSrc,
				lpDst,
				nSize);
		}
		__finally
		{
			s_eventLogger.LogGetEnvironmentVariable(
				lpSrc);
		}

		return rv;
	}

	DWORD WINAPI ExpandEnvironmentStringsW(
		PCWSTR lpSrc,
		PWCHAR lpDst,
		DWORD nSize)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::ExpandEnvironmentStringsW(
				lpSrc,
				lpDst,
				nSize);
		}
		__finally
		{
			s_eventLogger.LogGetEnvironmentVariable(
				lpSrc);
		}

		return rv;
	}

	DWORD WINAPI GetEnvironmentVariableA(
		PCSTR lpName,
		PCHAR lpBuffer,
		DWORD nSize)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::GetEnvironmentVariableA(
				lpName,
				lpBuffer,
				nSize);
		}
		__finally
		{
			s_eventLogger.LogGetEnvironmentVariable(
				lpName);
		}

		return rv;
	}

	DWORD WINAPI GetEnvironmentVariableW(
		PCWSTR lpName,
		PWCHAR lpBuffer,
		DWORD nSize)
	{
		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::GetEnvironmentVariableW(
				lpName,
				lpBuffer,
				nSize);
		}
		__finally
		{
			s_eventLogger.LogGetEnvironmentVariable(
				lpName);
		}

		return rv;
	}

	int WINAPI EntryPoint(void)
	{
		// This function is invoked instead of the process EntryPoint (Functions::Cache::EntryPoint).
		std::stringstream pipeName;
		pipeName << TBLOG_PIPE_NAMEA << "." << s_nTraceProcessId;
		s_eventLogger.Intialize(pipeName.str());

		return Functions::Cache::EntryPoint();
	}

	void WINAPI ExitProcess(UINT uExitCode)
	{
		Functions::Cache::ExitProcess(uExitCode);
	}
}