#pragma once 

void ThrowIfFailed(HRESULT hr, std::string_view message)
{
	if (hr != S_OK)
		throw std::runtime_error(message.data());
}

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

std::string s_szPipe;
LONG s_nActiveClients = 0;
LONG s_nTotalClients = 0;

VOID MyErrExit(PCSTR pszMsg)
{
	auto error = GetLastError();
	std::cerr << "Error " << error << " in " << pszMsg << "." << std::endl;
	exit(1);
}

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
		s_szPipe.c_str(),                   // pipe name
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
			pClient->fAwaitingAccept = false;
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
	else if (error == ERROR_BROKEN_PIPE)
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
	LPOVERLAPPED lpo;
	DWORD nBytes;
	HANDLE hCompletionPort = (HANDLE)pvVoid;

	bool isDone = false;
	while (!isDone)
	{
		// Wait for the next async write event on the pipeline
		pClient = nullptr;
		lpo = nullptr;
		nBytes = 0;
		if (GetQueuedCompletionStatus(
			hCompletionPort,
			&nBytes,
			(PULONG_PTR)&pClient,
			&lpo,
			INFINITE))
		{
			if (pClient->fAwaitingAccept)
			{
				std::cout << "Here1" << std::endl;
				bool fAgain = true;
				while (fAgain)
				{
					LONG nClient = InterlockedIncrement(&s_nTotalClients);
					InterlockedIncrement(&s_nActiveClients);
					pClient->fAwaitingAccept = false;

					CLIENT* pNew = CreatePipeConnection(hCompletionPort, nClient);

					fAgain = false;
					if (pNew != nullptr)
					{
						fAgain = !pNew->fAwaitingAccept;
						DoRead(pNew);
					}
				}
			}
			else
			{
				std::cout << "Here2" << std::endl;
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
		else
		{
			if (pClient)
			{
				std::cout << "Here3" << std::endl;
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
		}
	}

	std::cout << "Here4" << std::endl;
	return 0;
}

BOOL CreateWorkers(HANDLE hCompletionPort)
{
	for (int i = 0; i < 1; i++)
	{
		DWORD dwThread;
		auto threadHandle = CreateThread(nullptr, 0, WorkerThread, hCompletionPort, 0, &dwThread);
		if (!threadHandle)
		{
			throw std::runtime_error("Failed CreateThread WorkerThread");
		}

		CloseHandle(threadHandle);
	}

	return true;
}

class DetouredProcess
{
public:
	DetouredProcess()
	{
	}

	int RunProcess(
		const std::string& application,
		const std::vector<std::string>& arguments)
	{
		std::stringstream argumentsValue;
		argumentsValue << "\"" << application << "\"";
		for (auto& arg : arguments)
		{
			argumentsValue << " " << arg;
		}

		std::string argumentsString = argumentsValue.str();

		// Create a name for the pipe
		std::stringstream pipeName;
		pipeName << TBLOG_PIPE_NAMEA << "." << GetCurrentProcessId();
		s_szPipe = pipeName.str();

		// Create the completion port.
		HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, NULL, 0);
		if (hCompletionPort == nullptr)
		{
			MyErrExit("CreateIoCompletionPort");
		}

		// Create completion port worker threads.
		CreateWorkers(hCompletionPort);
		CreatePipeConnection(hCompletionPort, 0);

		auto moduleName = System::IProcessManager::Current().GetProcessFileName();
		auto moduleFolder = moduleName.GetParent();
		auto dllPath = moduleFolder + Path("Monitor.Detours.64.dll");
		auto dllPathString = dllPath.ToString();

		STARTUPINFOA si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);

		// Create the requested process with our detour dll loaded
		DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
		if (!DetourCreateProcessWithDllExA(
			application.c_str(),
			argumentsString.data(),
			nullptr,
			nullptr,
			true,
			dwFlags,
			nullptr,
			nullptr,
			&si,
			&pi,
			dllPathString.c_str(),
			nullptr))
		{
			throw std::runtime_error("DetourCreateProcessWithDllEx failed: " + std::to_string(GetLastError()));
		}

		Monitor::DetourPayload payload;
		ZeroMemory(&payload, sizeof(payload));
		payload.nParentProcessId = GetCurrentProcessId();
		payload.nTraceProcessId = GetCurrentProcessId();
		payload.nGeneology = 1;
		payload.rGeneology[0] = 0;
		StringCchCopyW(payload.wzParents, ARRAYSIZE(payload.wzParents), L"");
		if (!DetourCopyPayloadToProcess(
			pi.hProcess,
			Monitor::GuidTrace,
			&payload,
			sizeof(payload)))
		{
			throw std::runtime_error("DetourCopyPayloadToProcess failed: " + std::to_string(GetLastError()));
		}

		DWORD result = 0;
		auto hr = ResumeThread(pi.hThread);
		auto error = GetLastError();

		WaitForSingleObject(pi.hProcess, INFINITE);
		if (!GetExitCodeProcess(pi.hProcess, &result))
		{
			throw std::runtime_error("GetExitCodeProcess failed: " + std::to_string(GetLastError()));
		}

		return result;
	}
};