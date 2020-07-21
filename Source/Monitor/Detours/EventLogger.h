
#pragma once
#include "Helpers.h"
#include "Functions/FileApiCache.h"

class EventLogger
{
public:
	static void Initialize()
	{
		std::stringstream pipeNameBuilder;
		pipeNameBuilder << TBLOG_PIPE_NAMEA << "." << s_nTraceProcessId;
		auto pipeName = pipeNameBuilder.str();

		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		for (int retries = 0; retries < 10; retries++)
		{
			// Wait up to 1 seconds for a pipe to appear.
			auto timoutMilliseconds = 1000;
			if (WaitNamedPipeA(pipeName.c_str(), timoutMilliseconds) != 0)
			{
				// Attempt to open the pipe
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
					if (SetNamedPipeHandleState(s_pipeHandle, &pipeMode, nullptr, nullptr))
					{
						// All good!
						return;
					}
				}
			}
		}

		// Couldn't open pipe.
		throw std::runtime_error("Failed to open pipe for event logger.");
	}

	static void Shutdown()
	{
		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		if (s_pipeHandle != INVALID_HANDLE_VALUE)
		{
			Monitor::DetourMessage message;
			message.Type = Monitor::DetourMessageType::Info_Shutdown;
			message.ContentSize = 0;
			UnsafeWriteMessage(message);
			FlushFileBuffers(s_pipeHandle);
			CloseHandle(s_pipeHandle);
			s_pipeHandle = INVALID_HANDLE_VALUE;
		}
	}

	static void AppendValue(Monitor::DetourMessage& message, std::string_view value)
	{
		auto startIndex = message.ContentSize;
		auto size = value.size() + 1;
		message.ContentSize += size;
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for string value");

		value.copy(reinterpret_cast<char*>(message.Content + startIndex), size);
	}

	static void AppendValue(Monitor::DetourMessage& message, std::wstring_view value)
	{
		auto startIndex = message.ContentSize;
		auto size = 2 * (value.size() + 1);
		message.ContentSize += size;
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for string value");

		value.copy(reinterpret_cast<wchar_t*>(message.Content + startIndex), size);
	}

	static void AppendValue(Monitor::DetourMessage& message, uint32_t value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(uint32_t);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for int 32 value");

		*reinterpret_cast<uint32_t*>(message.Content + startIndex) = value;
	}

	static void WriteError(std::string_view value)
	{
		Monitor::DetourMessage message;
		message.Type = Monitor::DetourMessageType::Info_Error;
		message.ContentSize = 0;
		AppendValue(message, value);
		UnsafeWriteMessage(message);
	}

	static void WriteMessage(const Monitor::DetourMessage& message)
	{
		auto lock = std::lock_guard<std::mutex>(s_pipeMutex);
		UnsafeWriteMessage(message);
	}

private:
	static void UnsafeWriteMessage(const Monitor::DetourMessage& message)
	{
		if (s_pipeHandle == INVALID_HANDLE_VALUE)
			return; // TODO: A static dll init may do bad things before the main entry initialize

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
			throw std::runtime_error("Failed write event logger");
		}

		if (countBytesWritten != countBytesToWrite)
			throw std::runtime_error("Did not write the expected number of bytes");
	}

private:
	static std::mutex s_pipeMutex;
	static HANDLE s_pipeHandle;
};

/*static*/ std::mutex EventLogger::s_pipeMutex = std::mutex();
/*static*/ HANDLE EventLogger::s_pipeHandle = INVALID_HANDLE_VALUE;