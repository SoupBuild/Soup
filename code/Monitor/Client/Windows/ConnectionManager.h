
#pragma once
#include "ConnectionManagerBase.h"
#include "Functions/Cache/FileApi.h"

namespace Monitor::Windows
{
	class ConnectionManager : public ConnectionManagerBase
	{
	private:
	public:
		ConnectionManager() :
		 	ConnectionManagerBase(),
			pipeHandle(INVALID_HANDLE_VALUE)
		{
		}

	private:
		virtual void Connect(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManager::Connect");
			std::stringstream pipeNameBuilder;
			pipeNameBuilder << TBLOG_PIPE_NAMEA << "." << traceProcessId;
			auto pipeName = pipeNameBuilder.str();

			for (int retries = 0; retries < 10; retries++)
			{
				// Wait up to 1 seconds for a pipe to appear.
				auto timeoutMilliseconds = 1000;
				DebugTrace("ConnectionManager::Connect WaitNamedPipeA");
				if (WaitNamedPipeA(pipeName.c_str(), timeoutMilliseconds) != 0)
				{
					// Attempt to open the pipe
					DebugTrace("ConnectionManager::Connect CreateFileA");
					pipeHandle = Functions::Cache::FileApi::CreateFileA(
						pipeName.c_str(),
						GENERIC_WRITE,
						0,
						nullptr,
						OPEN_EXISTING,
						0,
						nullptr);
					if (pipeHandle != INVALID_HANDLE_VALUE)
					{
						DWORD pipeMode = PIPE_READMODE_MESSAGE;
						DebugTrace("ConnectionManager::Connect SetNamedPipeHandleState");
						if (SetNamedPipeHandleState(pipeHandle, &pipeMode, nullptr, nullptr))
						{
							// All good!
							return;
						}
						else
						{
							auto error = GetLastError();
							DebugTrace("ConnectionManager::Connect SetNamedPipeHandleState failed {}", error);
							throw std::runtime_error("SetNamedPipeHandleState failed with unknown error.");
						}
					}
				}
				else
				{
					auto error = GetLastError();
					switch (error)
					{
						case ERROR_SEM_TIMEOUT:
							// Keep waiting
							DebugTrace("ConnectionManager::Connect WaitNamedPipeA ERROR_SEM_TIMEOUT");
							break;
						default:
							DebugTrace("ConnectionManager::Connect WaitNamedPipeA Unknown error {}", error);
							throw std::runtime_error("WaitNamedPipeA failed with unknown error.");
					}
				}
			}

			// Couldn't open pipe.
			DebugTrace("ConnectionManager::Connect failed");
			throw std::runtime_error("Failed to open pipe for event logger.");
		}

		virtual void Disconnect()
		{
			if (pipeHandle != INVALID_HANDLE_VALUE)
			{
				FlushFileBuffers(pipeHandle);
				CloseHandle(pipeHandle);
				pipeHandle = INVALID_HANDLE_VALUE;
			}
		}

		virtual bool TryUnsafeWriteMessage(const Message& message)
		{
			if (pipeHandle == INVALID_HANDLE_VALUE)
			{
				DebugError("Handle not ready", (uint32_t)message.Type);
				return false;
			}

			// Write the message
			DWORD countBytesToWrite = message.ContentSize +
				sizeof(Message::Type) +
				sizeof(Message::ContentSize);
			DWORD countBytesWritten = 0;
			if (!Functions::Cache::FileApi::WriteFile(
				pipeHandle,
				&message,
				countBytesToWrite,
				&countBytesWritten,
				nullptr))
			{
				int error = GetLastError();
				(error);
				DebugError("Failed write event logger");
				return false;
			}

			if (countBytesWritten != countBytesToWrite)
			{
				DebugError("Did not write the expected number of bytes");
				return false;
			}

			return true;
		}

	private:
		HANDLE pipeHandle;
	};
}

// Create a singleton
static Monitor::Windows::ConnectionManager connectionManager;