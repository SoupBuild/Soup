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
		EnterFunc();

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

				Print("<t:Child id=\"::%hs::\">\n", szProc);

				WCHAR wzPath[MAX_PATH];
				FileInfo *pInfo = nullptr;
				if (lpApplicationName == nullptr)
				{
					PWCHAR pwzDst = wzPath;
					PWCHAR pwzSrc = lpCommandLine;

					if (*pwzSrc == '\"')
					{
						WCHAR cQuote = *pwzSrc++;

						while (*pwzSrc && *pwzSrc != cQuote)
						{
							*pwzDst++ = *pwzSrc++;
						}

						*pwzDst++ = '\0';
					}
					else
					{
						while (*pwzSrc && *pwzSrc != ' ' && *pwzSrc != '\t')
						{
							if (*pwzSrc == '\t')
							{
								*pwzSrc = ' ';
							}

							*pwzDst++ = *pwzSrc++;
						}

						*pwzDst++ = '\0';
					}

					pInfo = FileNames::FindPartial(wzPath);
				}
				else
				{
					pInfo = FileNames::FindPartial(lpApplicationName);
				}

				Print(
					"<t:Executable>%ls</t:Executable>\n",
					FileNames::ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo));
				Print("<t:Line>%le</t:Line>\n", lpCommandLine);
				Print("</t:Child>\n");

				if (pInfo)
				{
					pInfo->m_fAbsorbed = true;
				}

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
			ExitFunc();
			if (!rv)
			{
				Print(
					"<!-- Warning: CreateProcessW failed %d: %ls; %ls -->\n",
					GetLastError(),
					lpApplicationName,
					lpCommandLine);
			}
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
		EnterFunc();

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

				Print("<t:Child id=\"::%hs::\">\n", szProc);

				WCHAR wzPath[MAX_PATH];
				FileInfo *pInfo = nullptr;
				if (lpApplicationName == nullptr)
				{
					PCHAR pszDst = szProc;
					PCHAR pszSrc = lpCommandLine;

					if (*pszSrc == '\"')
					{
						CHAR cQuote = *pszSrc++;

						while (*pszSrc && *pszSrc != cQuote)
						{
							*pszDst++ = *pszSrc++;
						}

						*pszDst++ = '\0';
					}
					else
					{
						while (*pszSrc && *pszSrc != ' ' && *pszSrc != '\t')
						{
							if (*pszSrc == '\t')
							{
								*pszSrc = ' ';
							}

							*pszDst++ = *pszSrc++;
						}

						*pszDst++ = '\0';
					}

					pInfo = FileNames::FindPartial(szProc);
				}
				else
				{
					pInfo = FileNames::FindPartial(lpApplicationName);
				}

				Print(
					"<t:Executable>%ls</t:Executable>\n",
					FileNames::ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo));
				Print("<t:Line>%he</t:Line>\n", lpCommandLine);
				Print("</t:Child>\n");

				if (pInfo)
				{
					pInfo->m_fAbsorbed = true;
				}

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
			ExitFunc();
			if (!rv)
			{
				Print(
					"<!-- Warning: CreateProcessA failed %d: %hs; %hs -->\n",
					GetLastError(),
					lpApplicationName,
					lpCommandLine);
			}
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
		};

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
		};

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
		};

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
		};

		return rv;
	}

	BOOL WINAPI CreateHardLinkW(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes)
	{
		EnterFunc();

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
		};

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
		};

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
		};

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
		};

		return rv;
	}

	BOOL WINAPI CreateDirectoryW(
		LPCWSTR a0,
		LPSECURITY_ATTRIBUTES a1)
	{
		/* int nIndent = */ EnterFunc();
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryW(a0, a1);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				FileInfo *pInfo = FileNames::FindPartial(a0);
				pInfo->m_fDirectory = true;
			}
		};

		return rv;
	}

	BOOL WINAPI CreateDirectoryExW(
		LPCWSTR a0,
		LPCWSTR a1,
		LPSECURITY_ATTRIBUTES a2)
	{
		/* int nIndent = */ EnterFunc();
		bool rv = 0;
		__try
		{
			rv = Functions::Cache::CreateDirectoryExW(a0, a1, a2);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				FileInfo *pInfo = FileNames::FindPartial(a1);
				pInfo->m_fDirectory = true;
			}
		};

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
		};

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
		};

		return rv;
	}

	BOOL WINAPI DeleteFileW(LPCWSTR a0)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::DeleteFileW(a0);
		}
		__finally
		{
			ExitFunc();
	#if 0
			Print("<!-- DeleteFileW(%le -->\n", a0);
	#endif
			NoteDelete(a0);
		};

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
		};

		return rv;
	}

	DWORD WINAPI GetFileAttributesW(LPCWSTR a0)
	{
		EnterFunc();

		DWORD rv = 0;
		__try
		{
			rv = Functions::Cache::GetFileAttributesW(a0);
		}
		__finally
		{
			ExitFunc();
		};

		return rv;
	}

	BOOL WINAPI MoveFileWithProgressW(
		LPCWSTR a0,
		LPCWSTR a1,
		LPPROGRESS_ROUTINE a2,
		LPVOID a3,
		DWORD a4)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileWithProgressW(a0, a1, a2, a3, a4);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				NoteRead(a0);
				NoteWrite(a1);
			}
		};

		return rv;
	}

	BOOL WINAPI MoveFileA(
		LPCSTR a0,
		LPCSTR a1)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileA(a0, a1);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				NoteRead(a0);
				NoteCleanup(a0);
				NoteWrite(a1);
			}
		};

		return rv;
	}

	BOOL WINAPI MoveFileW(
		LPCWSTR a0,
		LPCWSTR a1)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileW(a0, a1);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				NoteRead(a0);
				NoteCleanup(a0);
				NoteWrite(a1);
			}
		};

		return rv;
	}

	BOOL WINAPI MoveFileExA(
		LPCSTR a0,
		LPCSTR a1,
		DWORD a2)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileExA(a0, a1, a2);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				NoteRead(a0);
				NoteCleanup(a0);
				NoteWrite(a1);
			}
		};

		return rv;
	}

	BOOL WINAPI MoveFileExW(
		LPCWSTR a0,
		LPCWSTR a1,
		DWORD a2)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::MoveFileExW(a0, a1, a2);
		}
		__finally
		{
			ExitFunc();
			if (rv)
			{
				NoteRead(a0);
				NoteCleanup(a0);
				NoteWrite(a1);
			}
		};

		return rv;
	}

	void SetHandle(PCSTR pszName, HANDLE h)
	{
	#if 0
		FileInfo *pInfo = OpenFiles::RecallFile(h);

		if (pInfo != nullptr)
		{
			Tblog("<!-- hset: %hs (%x) %ls -->\n", pszName, h, pInfo->m_pwzPath);
		}
		else
		{
			Tblog("<!-- hset: %hs (%x) ***Unknown*** -->\n", pszName, h);
		}
	#else
		(void)pszName;
		(void)h;
	#endif
	}

	BOOL WINAPI SetStdHandle(
		DWORD a0,
		HANDLE a1)
	{
		EnterFunc();

		bool rv = 0;
		__try
		{
			rv = Functions::Cache::SetStdHandle(a0, a1);
			if (rv && a1 != 0)
			{
				switch (a0)
				{
					case STD_INPUT_HANDLE:
						SetHandle("stdin", a1);
						break;
					case STD_OUTPUT_HANDLE:
						SetHandle("stdout", a1);
						break;
					case STD_ERROR_HANDLE:
						SetHandle("stderr", a1);
						break;
				}
			}
		}
		__finally
		{
			ExitFunc();
		};

		return rv;
	}

	HMODULE WINAPI LoadLibraryA(LPCSTR a0)
	{
		EnterFunc();

		HMODULE rv = 0;
		__try {
			rv = Functions::Cache::LoadLibraryA(a0);
		} __finally {
			ExitFunc();
		};
		return rv;
	}

	HMODULE WINAPI LoadLibraryW(LPCWSTR a0)
	{
		EnterFunc();

		HMODULE rv = 0;
		__try {
			rv = Functions::Cache::LoadLibraryW(a0);
		} __finally {
			ExitFunc();
		};
		return rv;
	}

	HMODULE WINAPI LoadLibraryExA(LPCSTR a0,
									HANDLE a1,
									DWORD a2)
	{
		EnterFunc();

		HMODULE rv = 0;
		__try {
			rv = Functions::Cache::LoadLibraryExA(a0, a1, a2);
		} __finally {
			ExitFunc();
		};
		return rv;
	}

	HMODULE WINAPI LoadLibraryExW(LPCWSTR a0,
									HANDLE a1,
									DWORD a2)
	{
		EnterFunc();

		HMODULE rv = 0;
		__try {
			rv = Functions::Cache::LoadLibraryExW(a0, a1, a2);
		} __finally {
			ExitFunc();
		};
		return rv;
	}

	DWORD WINAPI SetFilePointer(HANDLE hFile,
									LONG lDistanceToMove,
									PLONG lpDistanceToMoveHigh,
									DWORD dwMoveMethod)
	{
		EnterFunc();

		DWORD rv = 0;
		__try {
			rv = Functions::Cache::SetFilePointer(hFile,
									lDistanceToMove,
									lpDistanceToMoveHigh,
									dwMoveMethod);
		} __finally {
			LONG high = 0;
			if (lpDistanceToMoveHigh == nullptr) {
				lpDistanceToMoveHigh = &high;
			}

			FileInfo * pInfo = OpenFiles::RecallFile(hFile);
			if (pInfo != nullptr) {
				if (dwMoveMethod == FILE_END && lDistanceToMove == 0xffffffff) {
	#if 0
					Print("<!-- SetFilePointer(APPEND, %le) -->\n",
						pInfo->m_pwzPath);
	#endif
					pInfo->m_fAppend = true;
				}
	#if 0
				else if (dwMoveMethod == FILE_END) {
					Print("<!-- SetFilePointer(END:%08x:%08x, %le) -->\n",
						(int)lDistanceToMove,
						*lpDistanceToMoveHigh,
						pInfo->m_pwzPath);
				}
				else if (dwMoveMethod == FILE_BEGIN) {
					Print("<!-- SetFilePointer(BEG:%08x:%08x, %le) -->\n",
						(int)lDistanceToMove,
						*lpDistanceToMoveHigh,
						pInfo->m_pwzPath);
				}
				else if (dwMoveMethod == FILE_CURRENT) {
					Print("<!-- SetFilePointer(CUR:%08x:%08x, %le) -->\n",
						(int)lDistanceToMove,
						*lpDistanceToMoveHigh,
						pInfo->m_pwzPath);
				}
	#endif
			}
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI SetFilePointerEx(HANDLE hFile,
									LARGE_INTEGER liDistanceToMove,
									PLARGE_INTEGER lpNewFilePointer,
									DWORD dwMoveMethod)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::SetFilePointerEx(hFile,
									liDistanceToMove,
									lpNewFilePointer,
									dwMoveMethod);
		} __finally {
	#if 0
			FileInfo * pInfo = OpenFiles::RecallFile(hFile);
			if (pInfo != nullptr) {
				if (dwMoveMethod == FILE_END) {
					Print("<!-- SetFilePointerEx(END:%I64d, %le) -->\n",
						liDistanceToMove.QuadPart,
						pInfo->m_pwzPath);
				}
				else if (dwMoveMethod == FILE_BEGIN) {
					Print("<!-- SetFilePointerEx(BEG:%I64d, %le) -->\n",
						liDistanceToMove.QuadPart,
						pInfo->m_pwzPath);
				}
				else if (dwMoveMethod == FILE_CURRENT) {
					Print("<!-- SetFilePointerEx(CUR:%I64d, %le) -->\n",
						liDistanceToMove.QuadPart,
						pInfo->m_pwzPath);
				}
			}
	#endif
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI ReadFile(HANDLE a0,
							LPVOID a1,
							DWORD a2,
							LPDWORD a3,
							LPOVERLAPPED a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::ReadFile(a0, a1, a2, a3, a4);
		} __finally {
			if (rv) {
				OpenFiles::SetRead(a0, a2);
			}
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI ReadFileEx(HANDLE a0,
								LPVOID a1,
								DWORD a2,
								LPOVERLAPPED a3,
								LPOVERLAPPED_COMPLETION_ROUTINE a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::ReadFileEx(a0, a1, a2, a3, a4);
		} __finally {
			if (rv) {
				OpenFiles::SetRead(a0, a2);
			}
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI WriteFile(HANDLE a0,
							LPCVOID a1,
							DWORD a2,
							LPDWORD a3,
							LPOVERLAPPED a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::WriteFile(a0, a1, a2, a3, a4);
		} __finally {
			OpenFiles::SetWrite(a0, a2);
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI WriteFileEx(HANDLE a0,
								LPCVOID a1,
								DWORD a2,
								LPOVERLAPPED a3,
								LPOVERLAPPED_COMPLETION_ROUTINE a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::WriteFileEx(a0, a1, a2, a3, a4);
		} __finally {
			OpenFiles::SetWrite(a0, a2);
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI WriteConsoleA(HANDLE a0,
									const void* a1,
									DWORD a2,
									LPDWORD a3,
									LPVOID a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::WriteConsoleA(a0, a1, a2, a3, a4);
		} __finally {
			OpenFiles::SetWrite(a0, a2);
			ExitFunc();
		};
		return rv;
	}

	BOOL WINAPI WriteConsoleW(HANDLE a0,
									const void* a1,
									DWORD a2,
									LPDWORD a3,
									LPVOID a4)
	{
		EnterFunc();

		bool rv = 0;
		__try {
			rv = Functions::Cache::WriteConsoleW(a0, a1, a2, a3, a4);
		} __finally {
			OpenFiles::SetWrite(a0, a2);
			ExitFunc();
		};
		return rv;
	}

	//////////////////////////////////////////////////////////////////////////////
	//
	DWORD WINAPI ExpandEnvironmentStringsA(PCSTR lpSrc, PCHAR lpDst, DWORD nSize)
	{
		EnterFunc();
		DWORD rv = 0;
		__try {
			rv = Functions::Cache::ExpandEnvironmentStringsA(lpSrc, lpDst, nSize);
		}
		__finally {
			if (rv > 0) {
	#if 0
				Print("<!-- ExpandEnvironmentStringsA(%he) -->\n", lpSrc);
	#endif
			}
			ExitFunc();
		};
		return rv;
	}

	DWORD WINAPI ExpandEnvironmentStringsW(PCWSTR lpSrc, PWCHAR lpDst, DWORD nSize)
	{
		EnterFunc();
		DWORD rv = 0;
		__try {
			rv = Functions::Cache::ExpandEnvironmentStringsW(lpSrc, lpDst, nSize);
		}
		__finally {
			if (rv > 0) {
	#if 0
				Print("<!-- ExpandEnvironmentStringsW(%le) -->\n", lpSrc);
	#endif
			}
			ExitFunc();
		};
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
		};

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
		};

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