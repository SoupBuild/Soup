
#pragma once
#include "Helpers.h"
#include "Functions/FileApiCache.h"

class EventLogger
{
public:
	static void Initialize()
	{
		Connect();

		// Notify that we are connected
		Monitor::DetourMessage message;
		message.Type = Monitor::DetourMessageType::Info_Initialize;
		message.ContentSize = 0;
		WriteMessage(message);
	}

	static void Shutdown()
	{
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
			message.ContentSize += size;
			if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
				throw std::runtime_error("Message content too long for string value");

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
			message.ContentSize += size;
			if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
				throw std::runtime_error("Message content too long for string value");

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
			throw std::runtime_error("Message content too long for int value");

		*reinterpret_cast<void**>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, int value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(int);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for int value");

		*reinterpret_cast<int*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned int value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned int);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for unsigned int value");

		*reinterpret_cast<unsigned int*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned long value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned long);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for unsigned long value");

		*reinterpret_cast<unsigned long*>(message.Content + startIndex) = value;
	}

	static void AppendValue(Monitor::DetourMessage& message, unsigned long long value)
	{
		auto startIndex = message.ContentSize;
		message.ContentSize += sizeof(unsigned long long);
		if (message.ContentSize > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long for unsigned long long value");

		*reinterpret_cast<unsigned long long*>(message.Content + startIndex) = value;
	}

	static void WriteError(std::string_view value)
	{
		printf("DETOURS-ERROR: %s\n", value.data());

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

private:
	static void Connect()
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

	static void UnsafeWriteMessage(const Monitor::DetourMessage& message)
	{
		if (s_pipeHandle == INVALID_HANDLE_VALUE)
		{
			printf("DETOURS-ERROR: Handle not ready!! %u\n", (uint32_t)message.Type);
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
			printf("DETOURS-ERROR: Failed write event logger\n", (uint32_t)message.Type);
			exit(-1234);
		}

		if (countBytesWritten != countBytesToWrite)
		{
			printf("Did not write the expected number of bytes\n", (uint32_t)message.Type);
			exit(-1234);
		}
	}

private:
	static std::mutex s_pipeMutex;
	static HANDLE s_pipeHandle;
};

/*static*/ std::mutex EventLogger::s_pipeMutex = std::mutex();
/*static*/ HANDLE EventLogger::s_pipeHandle = INVALID_HANDLE_VALUE;