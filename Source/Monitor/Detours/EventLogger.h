
#pragma once
#include "Helpers.h"
#include "Functions/FileApiCache.h"

class EventLogger
{
public:
	static void Initialize()
	{
		DebugTrace("EventLogger::Initialize");
		Connect();

		// Notify that we are connected
		Monitor::DetourMessage message;
		message.Type = Monitor::DetourMessageType::Info_Initialize;
		message.ContentSize = 0;
		WriteMessage(message);
	}

	static void Shutdown()
	{
		DebugTrace("EventLogger::Shutdown");
		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		Monitor::DetourMessage message;
		message.Type = Monitor::DetourMessageType::Info_Shutdown;
		message.ContentSize = 0;
		UnsafeWriteMessage(message);

		if (s_pipeHandle != INVALID_HANDLE_VALUE)
		{
			FlushFileBuffers(s_pipeHandle);
			CloseHandle(s_pipeHandle);
			s_pipeHandle = INVALID_HANDLE_VALUE;
		}
	}

	static void AppendValue(Monitor::DetourMessage& message, const char* value)
	{
		if (value != nullptr)
		{
			auto stringValue = std::string_view(value);
			auto startIndex = message.ContentSize;
			auto size = stringValue.size() + 1;
			message.ContentSize += static_cast<uint32_t>(size);
			if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			{
				WriteError("Message content too long for const char* value");
				exit(-1234);
			}

			stringValue.copy(reinterpret_cast<char*>(message.Content + startIndex), size);
		}
		else
		{
			AppendValue(message, "");
		}
	}

	static void AppendValue(Monitor::DetourMessage& message, const wchar_t* value)
	{
		if (value != nullptr)
		{
			auto stringValue = std::wstring_view(value);
			auto startIndex = message.ContentSize;
			auto size = 2 * (stringValue.size() + 1);
			message.ContentSize += static_cast<uint32_t>(size);
			if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			{
				WriteError("Message content too long for const wchar_t* value");
				exit(-1234);
			}

			stringValue.copy(reinterpret_cast<wchar_t*>(message.Content + startIndex), size);
		}
		else
		{
			AppendValue(message, L"");
		}
	}

	static void AppendValue(Monitor::DetourMessage& message, void* value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(void*);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for void* value");
			exit(-1234);
		}

		*reinterpret_cast<void**>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, int value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(int);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for int value");
			exit(-1234);
		}

		*reinterpret_cast<int*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, long value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(long);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for long value");
			exit(-1234);
		}

		*reinterpret_cast<long*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned int value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned int);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for unsigned int value");
			exit(-1234);
		}

		*reinterpret_cast<unsigned int*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned long value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned long);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for unsigned long value");
			exit(-1234);
		}

		*reinterpret_cast<unsigned long*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned long long value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned long long);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
		{
			WriteError("Message content too long for unsigned long long value");
			exit(-1234);
		}

		*reinterpret_cast<unsigned long long*>(message.Content + startIndex) = value;
	}

	static void WriteError(std::string_view value)
	{
		DebugError(value.data());

		Monitor::DetourMessage message;
		message.Type = Monitor::DetourMessageType::Info_Error;
		message.ContentSize = 0;
		AppendValue(message, value.data());
		WriteMessage(message);
	}

	static void WriteMessage(const Monitor::DetourMessage& message)
	{
		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		UnsafeWriteMessage(message);
	}


	static void DebugError(std::string_view message, uint32_t value)
	{
		printf("DETOUR-CLIENT-ERROR: %s %u\n", message.data(), value);
	}

	static void DebugError(std::string_view message)
	{
		printf("DETOUR-CLIENT-ERROR: %s\n", message.data());
	}

	static void DebugTrace(std::string_view message)
	{
#ifdef TRACE_DETOUR_CLIENT
		printf("DETOUR-CLIENT: %s\n", message.data());
#else
		(message);
#endif
	}

private:
	static void Connect()
	{
		DebugTrace("EventLogger::Connect");
		std::stringstream pipeNameBuilder;
		pipeNameBuilder << TBLOG_PIPE_NAMEA << "." << s_nTraceProcessId;
		auto pipeName = pipeNameBuilder.str();

		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		for (int retries = 0; retries < 10; retries++)
		{
			// Wait up to 1 seconds for a pipe to appear.
			auto timoutMilliseconds = 1000;
			DebugTrace("EventLogger::Connect WaitNamedPipeA");
			if (WaitNamedPipeA(pipeName.c_str(), timoutMilliseconds) != 0)
			{
				// Attempt to open the pipe
				DebugTrace("EventLogger::Connect CreateFileA");
				s_pipeHandle = Functions::FileApi::Cache::CreateFileA(
					pipeName.c_str(),
					GENERIC_WRITE,
					0,
					nullptr,
					OPEN_EXISTING,
					0,
					nullptr);
				if (s_pipeHandle != INVALID_HANDLE_VALUE)
				{
					DWORD pipeMode = PIPE_READMODE_MESSAGE;
					DebugTrace("EventLogger::Connect SetNamedPipeHandleState");
					if (SetNamedPipeHandleState(s_pipeHandle, &pipeMode, nullptr, nullptr))
					{
						// All good!
						return;
					}
					else
					{
						auto error = GetLastError();
						DebugTrace("EventLogger::Connect SetNamedPipeHandleState failed" + std::to_string(error));
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
						DebugTrace("EventLogger::Connect WaitNamedPipeA ERROR_SEM_TIMEOUT");
						break;
					default:
						DebugTrace("EventLogger::Connect WaitNamedPipeA Unknown error " + std::to_string(error));
						throw std::runtime_error("WaitNamedPipeA failed with unknown error.");
				}
			}
		}

		// Couldn't open pipe.
		DebugTrace("EventLogger::Connect failed");
		throw std::runtime_error("Failed to open pipe for event logger.");
	}

	static void UnsafeWriteMessage(const Monitor::DetourMessage& message)
	{
		if (s_pipeHandle == INVALID_HANDLE_VALUE)
		{
			DebugError("Handle not ready", (uint32_t)message.Type);
			exit(-1234);
		}

		// Write the message
		DWORD countBytesToWrite = message.ContentSize +
			sizeof(Monitor::DetourMessage::Type) +
			sizeof(Monitor::DetourMessage::ContentSize);
		DWORD countBytesWritten = 0;
		if (!Functions::FileApi::Cache::WriteFile(
			s_pipeHandle,
			&message,
			countBytesToWrite,
			&countBytesWritten,
			nullptr))
		{
			DebugError("DETOURS-CLIENT-ERROR: Failed write event logger");
			exit(-1234);
		}

		if (countBytesWritten != countBytesToWrite)
		{
			DebugError("Did not write the expected number of bytes");
			exit(-1234);
		}
	}

private:
	static std::mutex s_pipeMutex;
	static HANDLE s_pipeHandle;
};

/*static*/ std::mutex EventLogger::s_pipeMutex = std::mutex();
/*static*/ HANDLE EventLogger::s_pipeHandle = INVALID_HANDLE_VALUE;