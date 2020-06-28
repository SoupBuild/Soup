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

		CHAR szProc[MAX_PATH];
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
				HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
				HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
				HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);

				if (lpStartupInfo != nullptr && (lpStartupInfo->dwFlags & STARTF_USESTDHANDLES) != 0)
				{
					hStdin = lpStartupInfo->hStdInput;
					hStdout = lpStartupInfo->hStdOutput;
					hStderr = lpStartupInfo->hStdError;
				}

				CreateProcessInternals(
					ppi->hProcess,
					ppi->dwProcessId,
					szProc,
					hStdin,
					hStdout,
					hStderr);

				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(ppi->hThread);
				}

				if (ppi == &pi)
				{
					Functions::Cache::CloseHandle(ppi->hThread);
					Functions::Cache::CloseHandle(ppi->hProcess);
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

		CHAR szProc[MAX_PATH];
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
				HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
				HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
				HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);

				if (lpStartupInfo != nullptr && (lpStartupInfo->dwFlags & STARTF_USESTDHANDLES) != 0)
				{
					hStdin = lpStartupInfo->hStdInput;
					hStdout = lpStartupInfo->hStdOutput;
					hStderr = lpStartupInfo->hStdError;
				}

				CreateProcessInternals(
					ppi->hProcess,
					ppi->dwProcessId,
					szProc,
					hStdin,
					hStdout,
					hStderr);

				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(ppi->hThread);
				}

				if (ppi == &pi)
				{
					Functions::Cache::CloseHandle(ppi->hThread);
					Functions::Cache::CloseHandle(ppi->hProcess);
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

	BOOL WINAPI CloseHandle(HANDLE a0)
	{
		/*int nIndent =*/ EnterFunc();

		bool rv = 0;
		__try
		{
			ProcInfo * pProc = OpenFiles::RecallProc(a0);
			if (pProc != nullptr)
			{
				Procs::Close(pProc->m_hProc);
			}

			FileInfo * pFile = OpenFiles::RecallFile(a0);
			if (pFile != nullptr)
			{
				DWORD dwErr = GetLastError();
				pFile->m_cbContent = GetFileSize(a0, nullptr);
				if (pFile->m_cbContent == INVALID_FILE_SIZE)
				{
					pFile->m_cbContent = 0;
				}

				if (pFile->m_fCantRead)
				{
					if (pFile->m_fRead)
					{
		#if 0
						Print("<!-- Warning: Removing read from %le -->\n", pFile->m_pwzPath);
		#endif
						pFile->m_fRead = false;
					}
				}

				// Here we should think about reading the file contents as appropriate.
				if (pFile->m_fTemporaryPath && pFile->m_fRead && !pFile->m_fAbsorbed &&
					!pFile->m_fDelete && !pFile->m_fCleanup && !pFile->m_fWrite &&
					pFile->m_pbContent == nullptr &&
					pFile->m_cbContent < 16384)
				{
					pFile->m_pbContent = LoadFile(a0, pFile->m_cbContent);
				}

				SetLastError(dwErr);
			}

			rv = Functions::Cache::CloseHandle(a0);
		}
		__finally
		{
			ExitFunc();
			if (rv /* && nIndent == 0*/)
			{
				OpenFiles::Forget(a0);
			}
		}

		return rv;
	}

	BOOL WINAPI DuplicateHandle(
		HANDLE hSourceProcessHandle,
		HANDLE hSourceHandle,
		HANDLE hTargetProcessHandle,
		LPHANDLE lpTargetHandle,
		DWORD dwDesiredAccess,
		bool bInheritHandle,
		DWORD dwOptions)
	{
		HANDLE hTemp = INVALID_HANDLE_VALUE;
		EnterFunc();

		bool rv = 0;
		__try
		{
			if (lpTargetHandle == nullptr)
			{
				lpTargetHandle = &hTemp;
			}

			*lpTargetHandle = INVALID_HANDLE_VALUE;

			rv = Functions::Cache::DuplicateHandle(
				hSourceProcessHandle,
				hSourceHandle,
				hTargetProcessHandle,
				lpTargetHandle,
				dwDesiredAccess,
				bInheritHandle,
				dwOptions);
		}
		__finally
		{
			ExitFunc();
			if (*lpTargetHandle != INVALID_HANDLE_VALUE)
			{
				FileInfo *pInfo = OpenFiles::RecallFile(hSourceHandle);
				if (pInfo)
				{
					OpenFiles::Remember(*lpTargetHandle, pInfo);
				}
			}
		}

		return rv;
	}

	static LONG s_nPipeCnt = 0;

	BOOL WINAPI CreatePipe(
		PHANDLE hReadPipe,
		PHANDLE hWritePipe,
		LPSECURITY_ATTRIBUTES lpPipeAttributes,
		DWORD nSize)
	{
		HANDLE hRead = INVALID_HANDLE_VALUE;
		HANDLE hWrite = INVALID_HANDLE_VALUE;

		if (hReadPipe == nullptr)
		{
			hReadPipe = &hRead;
		}

		if (hWritePipe == nullptr)
		{
			hWritePipe = &hWrite;
		}

		/*int nIndent = */ EnterFunc();
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreatePipe(hReadPipe, hWritePipe, lpPipeAttributes, nSize);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				CHAR szPipe[128];

				SafePrintf(
					szPipe,
					ARRAYSIZE(szPipe),
					"\\\\.\\PIPE\\Temp.%d.%d",
					GetCurrentProcessId(),
					InterlockedIncrement(&s_nPipeCnt));

				FileInfo *pInfo = FileNames::FindPartial(szPipe);

				pInfo->m_fCleanup = true;
				OpenFiles::Remember(*hReadPipe, pInfo);
				OpenFiles::Remember(*hWritePipe, pInfo);
			}
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

	HANDLE WINAPI CreateFileMappingW(
		HANDLE hFile,
		LPSECURITY_ATTRIBUTES a1,
		DWORD flProtect,
		DWORD a3,
		DWORD a4,
		LPCWSTR a5)
	{
		/* int nIndent = */ EnterFunc();
		HANDLE rv = 0;
		__try
		{
			rv = Functions::Cache::CreateFileMappingW(hFile, a1, flProtect, a3, a4, a5);
		}
		__finally
		{
			ExitFunc();
			if (rv != INVALID_HANDLE_VALUE)
			{
				FileInfo *pInfo = OpenFiles::RecallFile(hFile);

				if (pInfo != nullptr)
				{
					switch (flProtect)
					{
						case PAGE_READONLY:
							pInfo->m_fRead = true;
							break;
						case PAGE_READWRITE:
							pInfo->m_fRead = true;
							pInfo->m_fWrite = true;
							break;
						case PAGE_WRITECOPY:
							pInfo->m_fRead = true;
							break;
						case PAGE_EXECUTE_READ:
							pInfo->m_fRead = true;
							break;
						case PAGE_EXECUTE_READWRITE:
							pInfo->m_fRead = true;
							pInfo->m_fWrite = true;
							break;
					}
				}
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

	static void Dump(LPVOID pvData, DWORD cbData)
	{
		CHAR szBuffer[128];
		PBYTE pbData = (PBYTE)pvData;

		for (DWORD i = 0; i < cbData; i += 16)
		{
			PCHAR psz = szBuffer;
			psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), "%4d: ", i);

			for (DWORD j = i; j < i + 16; j++)
			{
				if (j < cbData)
				{
					psz = SafePrintf(
						psz,
						(LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz),
						"%02x",
						pbData[j]);
				}
				else
				{
					psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), "  ");
				}
			}

			for (DWORD j = i; j < i + 16; j++)
			{
				if (j < cbData)
				{
					if (pbData[j] >= ' ' && pbData[j] <= 127)
					{
						psz = SafePrintf(
							psz,
							(LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz),
							"%c",
							pbData[j]);
					}
					else
					{
						psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), ".");
					}
				}
				else
				{
					psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), " ");
				}
			}

			Print("%s\n", szBuffer);
		}
	}

	BOOL WINAPI DeviceIoControl(
		HANDLE a0,
		DWORD a1,
		LPVOID a2,
		DWORD a3,
		LPVOID a4,
		DWORD a5,
		LPDWORD a6,
		LPOVERLAPPED a7)
	{
		EnterFunc();
		DWORD d6 = 0;
		if (a6 == nullptr)
		{
			a6 = &d6;
		}

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::DeviceIoControl(a0, a1, a2, a3, a4, a5, a6, a7);
		}
		__finally
		{
			ExitFunc();
			OpenFiles::SetRead(a0, 0);
			OpenFiles::SetWrite(a0, 0);
			if (rv && a1 != 0x390008 && a1 != 0x4d0008 && a1 != 0x6d0008)
			{
				FileInfo *pInfo = OpenFiles::RecallFile(a0);

				DWORD DeviceType = (a1 & 0xffff0000) >> 16;
				DWORD Access = (a1 & 0x0000c000) >> 14;
				DWORD Function = (a1 & 0x00003ffc) >> 2;
				DWORD Method = (a1 & 0x00000003) >> 0;

				if (pInfo)
				{
					Print(
						"<!-- DeviceIoControl %x [dev=%x,acc=%x,fun=%x,mth=%x] on %ls! -->\n",
						a1,
						DeviceType,
						Access,
						Function,
						Method,
						pInfo->m_pwzPath);
				}
				else
				{
					Print(
						"<!-- DeviceIoControl %x [dev=%x,acc=%x,fun=%x,mth=%x,in=%d,out=%d/%d] on (%x)! -->\n",
						a1,
						DeviceType,
						Access,
						Function,
						Method,
						a3,
						*a6,
						a5,
						a0);

					if (a3 > 0)
					{
						Dump(a2, a3);
					}

					if (a5 > 0)
					{
						Dump(a4, (*a6 < a5) ? *a6 : a5);
					}
				}
			}
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
		TblogOpen();

		SaveEnvironment();

		TestHandle("t:StdIn", GetStdHandle(STD_INPUT_HANDLE));
		TestHandle("t:StdOut", GetStdHandle(STD_OUTPUT_HANDLE));
		TestHandle("t:StdErr", GetStdHandle(STD_ERROR_HANDLE));

		return Functions::Cache::EntryPoint();
	}

	void WINAPI ExitProcess(UINT uExitCode)
	{
		Functions::Cache::ExitProcess(uExitCode);
	}
}