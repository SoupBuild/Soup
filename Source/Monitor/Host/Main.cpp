import Monitor.Shared;
import Detours;

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103)
#endif
#include <strsafe.h>
#pragma warning(pop)

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "DetouredProcess.h"

struct CLIENT : OVERLAPPED
{
	HANDLE hPipe;
	LONG nClient;
	BOOL fAwaitingAccept;
	PVOID Zero;
	Monitor::DetourMessage Message;

	BOOL LogMessage(Monitor::DetourMessage* pMessage, DWORD nBytes);
	BOOL LogMessageV(PCHAR pszMsg, ...);
};

//////////////////////////////////////////////////////////////////////////////
//
CHAR s_szLogFile[MAX_PATH];
CHAR s_szPipe[MAX_PATH];
LONG s_nActiveClients = 0;
LONG s_nTotalClients = 0;
LONGLONG s_llStartTime;
Monitor::DetourPayload s_Payload;

//////////////////////////////////////////////////////////////////////////////
//
VOID MyErrExit(PCSTR pszMsg)
{
	auto error = GetLastError();
	std::cerr << "Error " << error << " in " << pszMsg << "." << std::endl;
	exit(1);
}

//////////////////////////////////////////////////////////////////////////////
//
BOOL CLIENT::LogMessageV(PCHAR pszMsg, ...)
{
	try
	{
		DWORD cbWritten = 0;
		CHAR szBuf[1024];
		PCHAR pcchEnd = szBuf + ARRAYSIZE(szBuf) - 2;
		PCHAR pcchCur = szBuf;
		HRESULT hr;

		va_list args;
		va_start(args, pszMsg);
		hr = StringCchVPrintfExA(
			pcchCur,
			pcchEnd - pcchCur,
			&pcchCur,
			nullptr,
			STRSAFE_NULL_ON_FAILURE,
			pszMsg,
			args);
		va_end(args);
		if (FAILED(hr))
		{
			throw std::runtime_error("StringCchVPrintfExA failed.");
		}

		hr = StringCchPrintfExA(
			pcchCur,
			szBuf + (ARRAYSIZE(szBuf)) - pcchCur,
			&pcchCur,
			nullptr,
			STRSAFE_NULL_ON_FAILURE,
			"\n");

		return true;
	}
	catch(...)
	{
		return false;
	}
}

BOOL CLIENT::LogMessage(Monitor::DetourMessage* pMessage, DWORD nBytes)
{
	// Sanity check the size of the message.
	if (nBytes > pMessage->ContentSize)
	{
		nBytes = pMessage->ContentSize;
	}

	if (nBytes >= sizeof(*pMessage))
	{
		nBytes = sizeof(*pMessage) - 1;
	}

	// Don't log message if there isn't and message text.
	DWORD cbWrite = nBytes - offsetof(Monitor::DetourMessage, Content);
	if (cbWrite <= 0)
	{
		return true;
	}

	switch (pMessage->Type)
	{
		case Monitor::DetourMessageType::Exit:
			std::cout << "Exit: ";
			break;
		case Monitor::DetourMessageType::Error:
			std::cout << "Error: ";
			break;
		case Monitor::DetourMessageType::CopyFile:
			std::cout << "CopyFile: ";
			break;
		case Monitor::DetourMessageType::CreateDirectory:
			std::cout << "CreateDirectory: ";
			break;
		case Monitor::DetourMessageType::CreateFile:
			std::cout << "CreateFile: ";
			break;
		case Monitor::DetourMessageType::CreateHardLink:
			std::cout << "CreateHardLink: ";
			break;
		case Monitor::DetourMessageType::CreateProcess:
			std::cout << "CreateProcess: ";
			break;
		case Monitor::DetourMessageType::DeleteFile:
			std::cout << "DeleteFile: ";
			break;
		case Monitor::DetourMessageType::GetEnvironmentVariable:
			std::cout << "GetEnvironmentVariable: ";
			break;
		case Monitor::DetourMessageType::GetFileAttributes:
			std::cout << "GetFileAttributes: ";
			break;
		case Monitor::DetourMessageType::LoadLibrary:
			std::cout << "LoadLibrary: ";
			break;
		case Monitor::DetourMessageType::MoveFile:
			std::cout << "MoveFile: ";
			break;
		case Monitor::DetourMessageType::OpenFile:
			std::cout << "OpenFile: ";
			break;
	}

	// Null terminate the string
	pMessage->Content[nBytes] = 0;
	std::cout << pMessage->Content << std::endl;

	return true;
}

BOOL CloseConnection(CLIENT* pClient)
{
	InterlockedDecrement(&s_nActiveClients);
	if (pClient != nullptr)
	{
		if (pClient->hPipe != INVALID_HANDLE_VALUE)
		{
			//FlushFileBuffers(pClient->hPipe);
			if (!DisconnectNamedPipe(pClient->hPipe))
			{
				DWORD error = GetLastError();
				pClient->LogMessageV("<!-- Error %d in DisconnectNamedPipe. -->\n", error);
			}

			CloseHandle(pClient->hPipe);
			pClient->hPipe = INVALID_HANDLE_VALUE;
		}

		GlobalFree(pClient);
		pClient = nullptr;
	}

	return true;
}

// Creates a pipe instance and initiate an accept request.
//
CLIENT* CreatePipeConnection(HANDLE hCompletionPort, LONG nClient)
{
	HANDLE hPipe = CreateNamedPipeA(
		s_szPipe,                   // pipe name
		PIPE_ACCESS_INBOUND |       // read-only access
		FILE_FLAG_OVERLAPPED,       // overlapped mode
		PIPE_TYPE_MESSAGE |         // message-type pipe
		PIPE_READMODE_MESSAGE |     // message read mode
		PIPE_WAIT,                  // blocking mode
		PIPE_UNLIMITED_INSTANCES,   // unlimited instances
		0,                          // output buffer size
		0,                          // input buffer size
		20000,                      // client time-out
		nullptr);                   // no security attributes
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		MyErrExit("CreateNamedPipe");
	}

	// Allocate the client data structure.
	//
	auto pClient = (CLIENT*)GlobalAlloc(GPTR, sizeof(CLIENT));
	if (pClient == nullptr)
	{
		MyErrExit("GlobalAlloc pClient");
	}

	ZeroMemory(pClient, sizeof(*pClient));
	pClient->hPipe = hPipe;
	pClient->nClient = nClient;
	pClient->fAwaitingAccept = true;

	// Associate file with our complietion port.
	//
	if (!CreateIoCompletionPort(pClient->hPipe, hCompletionPort, (ULONG_PTR)pClient, 0))
	{
		MyErrExit("CreateIoComplietionPort pClient");
	}

	if (!ConnectNamedPipe(hPipe, pClient))
	{
		DWORD error = GetLastError();

		if (error == ERROR_IO_PENDING)
		{
			return nullptr;
		}

		if (error == ERROR_PIPE_CONNECTED)
		{
#if 0
			pClient->LogMessageV("<!-- ConnectNamedPipe client already connected. -->");
#endif
			pClient->fAwaitingAccept = FALSE;
		}
		else if (error != ERROR_IO_PENDING &&
				error != ERROR_PIPE_LISTENING)
		{

			MyErrExit("ConnectNamedPipe");
		}
	}
	else
	{
		fprintf(stderr, "*** ConnectNamedPipe accepted immediately.\n");
#if 0
		pClient->LogMessageV("<!-- ConnectNamedPipe accepted immediately. -->");
#endif
		pClient->fAwaitingAccept = FALSE;
	}

	return pClient;
}

BOOL DoRead(CLIENT* pClient)
{
	SetLastError(NO_ERROR);
	DWORD nBytes = 0;
	BOOL b = ReadFile(
		pClient->hPipe,
		&pClient->Message,
		sizeof(pClient->Message),
		&nBytes,
		pClient);

	DWORD error = GetLastError();

	if (b && error == NO_ERROR)
	{
		return true;
	}
	if (error == ERROR_BROKEN_PIPE)
	{
		pClient->LogMessageV("<!-- **** ReadFile 002 *** ERROR_BROKEN_PIPE [%d] -->\n", nBytes);
		CloseConnection(pClient);
		return true;
	}
	else if (error == ERROR_INVALID_HANDLE)
	{
		// ?
		pClient->LogMessageV("<!-- **** ReadFile 002 *** ERROR_INVALID_HANDLE -->\n");
		// I have no idea why this happens.  Our remedy is to drop the connection.
		return true;
	}
	else if (error != ERROR_IO_PENDING)
	{
		if (b)
		{
			pClient->LogMessageV("<!-- **** ReadFile 002 succeeded: %d -->\n", error);
		}
		else
		{
			pClient->LogMessageV("<!-- **** ReadFile 002 failed: %d -->\n", error);
		}

		CloseConnection(pClient);
	}

	return true;
}

DWORD WINAPI WorkerThread(LPVOID pvVoid)
{
	CLIENT* pClient;
	BOOL b;
	LPOVERLAPPED lpo;
	DWORD nBytes;
	HANDLE hCompletionPort = (HANDLE)pvVoid;

	for (BOOL fKeepLooping = true; fKeepLooping;)
	{
		pClient = nullptr;
		lpo = nullptr;
		nBytes = 0;
		b = GetQueuedCompletionStatus(
			hCompletionPort,
			&nBytes,
			(PULONG_PTR)&pClient,
			&lpo,
			INFINITE);

		if (!b)
		{
			if (pClient)
			{
				if (GetLastError() == ERROR_BROKEN_PIPE)
				{
					pClient->LogMessageV("<!-- Client closed pipe. -->");
				}
				else
				{
					pClient->LogMessageV(
						"<!-- *** GetQueuedCompletionStatus failed %d -->",
						GetLastError());
				}

				CloseConnection(pClient);
			}

			continue;
		}

		if (pClient->fAwaitingAccept)
		{
			BOOL fAgain = true;
			while (fAgain)
			{
				LONG nClient = InterlockedIncrement(&s_nTotalClients);
				InterlockedIncrement(&s_nActiveClients);
				pClient->fAwaitingAccept = FALSE;

				CLIENT* pNew = CreatePipeConnection(hCompletionPort, nClient);

				fAgain = FALSE;
				if (pNew != nullptr)
				{
					fAgain = !pNew->fAwaitingAccept;
					DoRead(pNew);
				}
			}
		}
		else
		{
			auto offset = offsetof(Monitor::DetourMessage, Content);
			if (nBytes <= offset)
			{
				pClient->LogMessageV("</t:Process>\n");
				CloseConnection(pClient);
				continue;
			}

			pClient->LogMessage(&pClient->Message, nBytes - offset);
		}

		DoRead(pClient);
	}

	return 0;
}

BOOL CreateWorkers(HANDLE hCompletionPort)
{
	DWORD dwThread;
	HANDLE hThread;
	DWORD i;
	SYSTEM_INFO SystemInfo;

	GetSystemInfo(&SystemInfo);

	for (i = 0; i < 1; i++)
	{
		hThread = CreateThread(nullptr, 0, WorkerThread, hCompletionPort, 0, &dwThread);
		if (!hThread)
		{
			MyErrExit("CreateThread WorkerThread");
			// Unreachable: return FALSE;
		}

		CloseHandle(hThread);
	}

	return true;
}

DWORD CopyEnvironment(PWCHAR pwzzOut, PCWSTR pwzzIn)
{
	PCWSTR pwzzBeg = pwzzOut;
	while (*pwzzIn)
	{
		while (*pwzzIn)
		{
			*pwzzOut++ = *pwzzIn++;
		}

		*pwzzOut++ = *pwzzIn++;   // Copy zero.
	}

	*pwzzOut++ = '\0';    // Add last zero.

	return (DWORD)(pwzzOut - pwzzBeg);
}

DWORD main(int argc, char **argv)
{
	HANDLE hCompletionPort;
	BOOL fNeedHelp = FALSE;

	GetSystemTimeAsFileTime((FILETIME *)&s_llStartTime);
	StringCchPrintfA(
		s_szPipe,
		ARRAYSIZE(s_szPipe),
		"%s.%d",
		TBLOG_PIPE_NAME,
		GetCurrentProcessId());

	int arg = 1;

	// Create the completion port.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 0);
	if (hCompletionPort == nullptr)
	{
		MyErrExit("CreateIoCompletionPort");
	}

	// Create completion port worker threads.
	CreateWorkers(hCompletionPort);
	CreatePipeConnection(hCompletionPort, 0);

	CHAR szTmpPath[MAX_PATH];
	CHAR szExePath[MAX_PATH];
	CHAR szDllPath[MAX_PATH];
	PCHAR pszFilePart = nullptr;

	if (!GetModuleFileNameA(nullptr, szTmpPath, ARRAYSIZE(szTmpPath)))
	{
		printf("Couldn't retreive exe name.\n");
		return 9002;
	}

	if (!GetFullPathNameA(szTmpPath, ARRAYSIZE(szExePath), szExePath, &pszFilePart) ||
		pszFilePart == nullptr)
	{
		printf("Error: %s is not a valid path name..\n", szTmpPath);
		return 9002;
	}

	StringCchCopyA(
		pszFilePart,
		szExePath + ARRAYSIZE(szExePath) - pszFilePart,
		"Monitor.Detours.dll");
	StringCchCopyA(szDllPath, ARRAYSIZE(szDllPath), szExePath);

	//////////////////////////////////////////////////////////////////////////
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	CHAR szCommand[2048];
	CHAR szExe[MAX_PATH];
	CHAR szFullExe[MAX_PATH] = "\0";
	PCHAR pszFileExe = nullptr;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	szCommand[0] = L'\0';

	StringCchCopyA(szExe, sizeof(szExe), argv[arg]);
	for (; arg < argc; arg++)
	{
		if (strchr(argv[arg], ' ') != nullptr || strchr(argv[arg], '\t') != nullptr)
		{
			StringCchCatA(szCommand, sizeof(szCommand), "\"");
			StringCchCatA(szCommand, sizeof(szCommand), argv[arg]);
			StringCchCatA(szCommand, sizeof(szCommand), "\"");
		}
		else
		{
			StringCchCatA(szCommand, sizeof(szCommand), argv[arg]);
		}

		if (arg + 1 < argc)
		{
			StringCchCatA(szCommand, sizeof(szCommand), " ");
		}
	}

	std::cout << "Starting: '" << szCommand << "'" << std::endl;
	std::cout << "  with '" << szDllPath << "'" << std::endl;

	DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

	SetLastError(0);
	SearchPathA(nullptr, szExe, ".exe", ARRAYSIZE(szFullExe), szFullExe, &pszFileExe);

	// TODO: Use DetourCreateProcessWithDllExA with 32 vs 64 support
	if (!DetourCreateProcessWithDllExA(
		szFullExe[0] ? szFullExe : nullptr,
		szCommand,
		nullptr,
		nullptr,
		true,
		dwFlags,
		nullptr,
		nullptr,
		&si,
		&pi,
		szDllPath,
		nullptr))
	{
		std::cout << "DetourCreateProcessWithDllEx failed: " << GetLastError() << std::endl;
		return 9007;
	}

	ZeroMemory(&s_Payload, sizeof(s_Payload));
	s_Payload.nParentProcessId = GetCurrentProcessId();
	s_Payload.nTraceProcessId = GetCurrentProcessId();
	s_Payload.nGeneology = 1;
	s_Payload.rGeneology[0] = 0;
	StringCchCopyW(s_Payload.wzParents, ARRAYSIZE(s_Payload.wzParents), L"");

	if (!DetourCopyPayloadToProcess(
		pi.hProcess,
		Monitor::GuidTrace,
		&s_Payload,
		sizeof(s_Payload)))
	{
		std::cout << "DetourCopyPayloadToProcess failed: " << GetLastError() << std::endl;
		return 9008;
	}

	DWORD dwResult = 0;
	auto hr = ResumeThread(pi.hThread);
	auto error = GetLastError();

	WaitForSingleObject(pi.hProcess, INFINITE);

	if (!GetExitCodeProcess(pi.hProcess, &dwResult))
	{
		printf("GetExitCodeProcess failed: %d\n", GetLastError());
		return 9008;
	}

	printf("%d processes.\n", s_nTotalClients);

	return dwResult;
}
