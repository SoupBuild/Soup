
#pragma once
#include "../ConnectionManager.h"

namespace Monitor
{
	class LinuxConnectionManager : public ConnectionManager
	{
	public:
		LinuxConnectionManager() :
		 	ConnectionManager(),
			pipeHandle()
		{
		}

	protected:
		virtual void Connect(int32_t traceProcessId)
		{
			DebugTrace("ConnectionManager::Connect");
			// std::stringstream pipeNameBuilder;
			// pipeNameBuilder << TBLOG_PIPE_NAMEA << "." << traceProcessId;
			// auto pipeName = pipeNameBuilder.str();

			// auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
			// for (int retries = 0; retries < 10; retries++)
			// {
			// 	// Wait up to 1 seconds for a pipe to appear.
			// 	auto timoutMilliseconds = 1000;
			// 	DebugTrace("ConnectionManager::Connect WaitNamedPipeA");
			// 	if (WaitNamedPipeA(pipeName.c_str(), timoutMilliseconds) != 0)
			// 	{
			// 		// Attempt to open the pipe
			// 		DebugTrace("ConnectionManager::Connect CreateFileA");
			// 		s_pipeHandle = Functions::FileApi::Cache::CreateFileA(
			// 			pipeName.c_str(),
			// 			GENERIC_WRITE,
			// 			0,
			// 			nullptr,
			// 			OPEN_EXISTING,
			// 			0,
			// 			nullptr);
			// 		if (s_pipeHandle != INVALID_HANDLE_VALUE)
			// 		{
			// 			DWORD pipeMode = PIPE_READMODE_MESSAGE;
			// 			DebugTrace("ConnectionManager::Connect SetNamedPipeHandleState");
			// 			if (SetNamedPipeHandleState(s_pipeHandle, &pipeMode, nullptr, nullptr))
			// 			{
			// 				// All good!
			// 				return;
			// 			}
			// 			else
			// 			{
			// 				auto error = GetLastError();
			// 				DebugTrace("ConnectionManager::Connect SetNamedPipeHandleState failed" + std::to_string(error));
			// 				throw std::runtime_error("SetNamedPipeHandleState failed with unknown error.");
			// 			}
			// 		}
			// 	}
			// 	else
			// 	{
			// 		auto error = GetLastError();
			// 		switch (error)
			// 		{
			// 			case ERROR_SEM_TIMEOUT:
			// 				// Keep waiting
			// 				DebugTrace("ConnectionManager::Connect WaitNamedPipeA ERROR_SEM_TIMEOUT");
			// 				break;
			// 			default:
			// 				DebugTrace("ConnectionManager::Connect WaitNamedPipeA Unknown error " + std::to_string(error));
			// 				throw std::runtime_error("WaitNamedPipeA failed with unknown error.");
			// 		}
			// 	}
			// }

			// // Couldn't open pipe.
			// DebugTrace("ConnectionManager::Connect failed");
			// throw std::runtime_error("Failed to open pipe for event logger.");
		}

		virtual void Disconnect()
		{
			// if (s_pipeHandle != INVALID_HANDLE_VALUE)
			// {
			// 	FlushFileBuffers(s_pipeHandle);
			// 	CloseHandle(s_pipeHandle);
			// 	s_pipeHandle = INVALID_HANDLE_VALUE;
			// }
		}

		virtual void UnsafeWriteMessage(const Monitor::Message& message)
		{
			// if (s_pipeHandle == INVALID_HANDLE_VALUE)
			// {
			// 	DebugError("Handle not ready", (uint32_t)message.Type);
			// 	exit(-1234);
			// }

			// // Write the message
			// DWORD countBytesToWrite = message.ContentSize +
			// 	sizeof(Monitor::Message::Type) +
			// 	sizeof(Monitor::Message::ContentSize);
			// DWORD countBytesWritten = 0;
			// if (!Functions::FileApi::Cache::WriteFile(
			// 	s_pipeHandle,
			// 	&message,
			// 	countBytesToWrite,
			// 	&countBytesWritten,
			// 	nullptr))
			// {
			// 	DebugError("DETOURS-CLIENT-ERROR: Failed write event logger");
			// 	exit(-1234);
			// }

			// if (countBytesWritten != countBytesToWrite)
			// {
			// 	DebugError("Did not write the expected number of bytes");
			// 	exit(-1234);
			// }
		}

	private:
		int pipeHandle;
	};
}