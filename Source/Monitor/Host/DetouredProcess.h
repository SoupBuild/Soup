#pragma once 

void ThrowIfFailed(HRESULT hr, std::string_view message)
{
	if (hr != S_OK)
		throw std::runtime_error(message.data());
}

std::string s_szPipe;
LONG s_nActiveClients = 0;
LONG s_nTotalClients = 0;

VOID MyErrExit(PCSTR pszMsg)
{
	auto error = GetLastError();
	std::cerr << "Error " << error << " in " << pszMsg << "." << std::endl;
	exit(1);
}

namespace Monitor
{
	class DetouredProcess
	{
	public:
		DetouredProcess()
		{
		}

		int RunProcess(
			const Path& application,
			const std::vector<std::string>& arguments)
		{
			auto applicationString = application.ToAlternateString();

			std::stringstream argumentsValue;
			argumentsValue << "\"" << applicationString << "\"";
			for (auto& arg : arguments)
			{
				argumentsValue << " " << arg;
			}

			std::string argumentsString = argumentsValue.str();

			// Create a name for the pipe
			std::stringstream pipeName;
			pipeName << TBLOG_PIPE_NAMEA << "." << GetCurrentProcessId();
			s_szPipe = pipeName.str();

			// Create completion port worker threads.
			CreatePipeConnection();
			auto workerThread = std::thread(&DetouredProcess::WorkerThread, *this);

			// Build up the detour dlls absolute path
			auto moduleName = System::IProcessManager::Current().GetProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto dllPath = moduleFolder + Path("Monitor.Detours.64.dll");
			auto dllPathString = dllPath.ToAlternateString();

			// Create the requested process with our detour dll loaded
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			si.cb = sizeof(si);
			DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
			if (!DetourCreateProcessWithDllExA(
				applicationString.c_str(),
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
				auto error = GetLastError();
				throw std::runtime_error("DetourCreateProcessWithDllEx failed: " + std::to_string(error));
			}

			// Set the detoured process payload
			DetourPayload payload;
			ZeroMemory(&payload, sizeof(payload));
			payload.nParentProcessId = GetCurrentProcessId();
			payload.nTraceProcessId = GetCurrentProcessId();
			payload.nGeneology = 1;
			payload.rGeneology[0] = 0;
			StringCchCopyW(payload.wzParents, ARRAYSIZE(payload.wzParents), L"");
			if (!DetourCopyPayloadToProcess(
				pi.hProcess,
				GuidTrace,
				&payload,
				sizeof(payload)))
			{
				throw std::runtime_error("DetourCopyPayloadToProcess failed: " + std::to_string(GetLastError()));
			}

			// Restert the process
			auto hr = ResumeThread(pi.hThread);

			// Wait for the process to finish
			WaitForSingleObject(pi.hProcess, INFINITE);

			// Get the output result
			DWORD result = 0;
			if (!GetExitCodeProcess(pi.hProcess, &result))
			{
				throw std::runtime_error("GetExitCodeProcess failed: " + std::to_string(GetLastError()));
			}

			// Wait for the worker thread to exit
			workerThread.join();
			return result;
		}

	private:
		void LogMessage(DetourMessage* pMessage, DWORD nBytes)
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
			DWORD cbWrite = nBytes - offsetof(DetourMessage, Content);
			if (cbWrite <= 0)
			{
				throw std::runtime_error("Not enough data.");
			}

			switch (pMessage->Type)
			{
				case DetourMessageType::Exit:
					std::cout << "Exit: ";
					break;
				case DetourMessageType::Error:
					std::cout << "Error: ";
					break;
				case DetourMessageType::CopyFile:
					std::cout << "CopyFile: ";
					break;
				case DetourMessageType::CreateDirectory:
					std::cout << "CreateDirectory: ";
					break;
				case DetourMessageType::CreateFile:
					std::cout << "CreateFile: ";
					break;
				case DetourMessageType::CreateHardLink:
					std::cout << "CreateHardLink: ";
					break;
				case DetourMessageType::CreateProcess:
					std::cout << "CreateProcess: ";
					break;
				case DetourMessageType::DeleteFile:
					std::cout << "DeleteFile: ";
					break;
				case DetourMessageType::GetEnvironmentVariable:
					std::cout << "GetEnvironmentVariable: ";
					break;
				case DetourMessageType::GetFileAttributes:
					std::cout << "GetFileAttributes: ";
					break;
				case DetourMessageType::LoadLibrary:
					std::cout << "LoadLibrary: ";
					break;
				case DetourMessageType::MoveFile:
					std::cout << "MoveFile: ";
					break;
				case DetourMessageType::OpenFile:
					std::cout << "OpenFile: ";
					break;
			}

			// Null terminate the string
			pMessage->Content[nBytes] = 0;
			std::cout << pMessage->Content << std::endl;
		}

		void CloseConnection()
		{
			InterlockedDecrement(&s_nActiveClients);
			if (m_hPipe != INVALID_HANDLE_VALUE)
			{
				//FlushFileBuffers(m_hPipe);
				if (!DisconnectNamedPipe(m_hPipe))
				{
					DWORD error = GetLastError();
					throw std::runtime_error("DisconnectNamedPipe Failed: " + std::to_string(error));
				}

				CloseHandle(m_hPipe);
				m_hPipe = INVALID_HANDLE_VALUE;
			}
		}

		void CreatePipeConnection()
		{
			// Open in Read-Only mode with synchronous operations enabled
			// Turn on message type pipe in read mode with blocking waits
			DWORD openMode = PIPE_ACCESS_INBOUND;
			DWORD pipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
			DWORD maxInstances = PIPE_UNLIMITED_INSTANCES;
			DWORD outBufferSize = 0;
			DWORD inBufferSize = 0;
			DWORD defaultTimeOut = 20000;
			HANDLE hPipe = CreateNamedPipeA(
				s_szPipe.c_str(),
				openMode,
				pipeMode,
				maxInstances,
				outBufferSize,
				inBufferSize,
				defaultTimeOut,
				nullptr);
			if (hPipe == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				throw std::runtime_error("CreateNamedPipeA failed: " + std::to_string(error));
			}

			m_hPipe = hPipe;
		}

		void WorkerThread()
		{
			// Connect to the pipe
			if (!ConnectNamedPipe(m_hPipe, nullptr))
			{
				// If completed then check the actual error
				DWORD error = GetLastError();
				switch (error)
				{
				case ERROR_PIPE_CONNECTED:
					// All good
					break;
				default:
					throw std::runtime_error("ConnectNamedPipe failed: " + std::to_string(error));
				}
			}

			// Read until we run out of messages
			bool isDone = false;
			while (!isDone)
			{
				// Read the next message
				DWORD bytesRead = 0;
				if (!ReadFile(
					m_hPipe,
					&m_Message,
					sizeof(m_Message),
					&bytesRead,
					nullptr))
				{
					DWORD error = GetLastError();
					throw std::runtime_error("ReadFile failed: " + std::to_string(error));
				}

				auto offset = offsetof(Monitor::DetourMessage, Content);
				if (bytesRead <= offset)
				{
					isDone = true;
				}
				else
				{
					LogMessage(&m_Message, bytesRead - offset);
				}
			}

			CloseConnection();
		}

	private:
		HANDLE m_hPipe;
		DetourMessage m_Message;
	};
}
