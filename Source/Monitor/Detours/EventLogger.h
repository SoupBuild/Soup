
#pragma once

class EventLogger
{
public:
	EventLogger() :
		m_pipeMutex(),
		m_pipeHandle(INVALID_HANDLE_VALUE)
	{
	}

	void Intialize(std::string_view pipeName)
	{
		auto lock = std::lock_guard<std::mutex>(m_pipeMutex);
		for (int retries = 0; retries < 10; retries++)
		{
			// Wait up to 10 seconds for a pipe to appear.
			auto timoutMilliseconds = 10000;
			if (WaitNamedPipeA(pipeName.data(), timoutMilliseconds) != 0)
			{
				// Attempt to open the pipe
				m_pipeHandle = Functions::Cache::CreateFileA(
					pipeName.data(),
					GENERIC_WRITE,
					0,
					nullptr,
					OPEN_EXISTING,
					0,
					nullptr);
				if (m_pipeHandle != INVALID_HANDLE_VALUE)
				{
					DWORD pipeMode = PIPE_READMODE_MESSAGE;
					if (SetNamedPipeHandleState(m_pipeHandle, &pipeMode, nullptr, nullptr))
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

	void LogCopyFile(std::string_view existingFileName, std::string_view newFileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CopyFile: " << existingFileName << " -> " << newFileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogCreateFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::stringstream messageBuilder;
		messageBuilder << "CreateFile: " << converter.to_bytes(fileName.data()) << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

private:
	void WriteMessage(std::string_view content)
	{
		auto lock = std::lock_guard<std::mutex>(m_pipeMutex);
		if (m_pipeHandle == INVALID_HANDLE_VALUE)
			throw std::runtime_error("Pipe not initialized.");
			
		TBLOG_MESSAGE message;
		message.nBytes = content.size() + sizeof(DWORD);
		std::copy(content.begin(), content.end(), message.szMessage);
		message.szMessage[content.size()] = 0;

		// Write the message
		DWORD countBytesWritten = 0;
		if (!Functions::Cache::WriteFile(
			m_pipeHandle,
			&message,
			message.nBytes,
			&countBytesWritten,
			nullptr))
		{
			throw std::runtime_error("Failed write event logger.");
		}
	}

private:
	std::mutex m_pipeMutex;
	HANDLE m_pipeHandle;
};
