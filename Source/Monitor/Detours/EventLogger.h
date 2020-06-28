
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

	void Shutdown()
	{
		auto lock = std::lock_guard<std::mutex>(m_pipeMutex);
		if (m_pipeHandle != INVALID_HANDLE_VALUE)
		{
			DWORD cbWritten = 0;
			DWORD emptyMessage = 0;
			Functions::Cache::WriteFile(m_pipeHandle, &emptyMessage, 4, &cbWritten, nullptr);
			FlushFileBuffers(m_pipeHandle);
			CloseHandle(m_pipeHandle);
			m_pipeHandle = INVALID_HANDLE_VALUE;
		}
	}

	void LogCopyFile(std::wstring_view existingFileName, std::wstring_view newFileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortExistingFileName = converter.to_bytes(existingFileName.data());
		auto shortNewFileName = converter.to_bytes(newFileName.data());
		LogCopyFile(shortExistingFileName, shortNewFileName);
	}

	void LogCopyFile(std::string_view existingFileName, std::string_view newFileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CopyFile: " << existingFileName << " -> " << newFileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogCreateDirectory(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateDirectory(shortFileName);
	}

	void LogCreateDirectory(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CreateDirectory: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogCreateHardLink(std::wstring_view fileName, std::wstring_view existingFileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		auto shortExistingFileName = converter.to_bytes(existingFileName.data());
		LogCreateHardLink(shortFileName, shortExistingFileName);
	}

	void LogCreateHardLink(std::string_view fileName, std::string_view existingFileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CreateHardLink: " << fileName << " -> " << existingFileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogCreateFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateFile(shortFileName);
	}

	void LogCreateFile(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CreateFile: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogCreateProcess(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateProcess(shortFileName);
	}

	void LogCreateProcess(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "CreateProcess: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogDeleteFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogDeleteFile(shortFileName);
	}

	void LogDeleteFile(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "DeleteFile: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogGetEnvironmentVariable(std::wstring_view name)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortName = converter.to_bytes(name.data());
		LogGetEnvironmentVariable(shortName);
	}

	void LogGetEnvironmentVariable(std::string_view name)
	{
		std::stringstream messageBuilder;
		messageBuilder << "GetEnvironmentVariable: " << name << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogGetFileAttributes(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogGetFileAttributes(shortFileName);
	}

	void LogGetFileAttributes(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "GetFileAttributes: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogLoadLibrary(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogLoadLibrary(shortFileName);
	}

	void LogLoadLibrary(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "LoadLibrary: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogMoveFile(std::wstring_view existingFileName, std::wstring_view newFileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortExistingFileName = converter.to_bytes(existingFileName.data());
		auto shortNewFileName = converter.to_bytes(newFileName.data());
		LogMoveFile(shortExistingFileName, shortNewFileName);
	}

	void LogMoveFile(std::string_view existingFileName, std::string_view newFileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "MoveFile: " << existingFileName << " -> " << newFileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogOpenFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogOpenFile(shortFileName);
	}

	void LogOpenFile(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "OpenFile: " << fileName << "\n";
		auto content = messageBuilder.str();

		WriteMessage(content);
	}

	void LogError(std::string_view fileName)
	{
		std::stringstream messageBuilder;
		messageBuilder << "Error: " << fileName << "\n";
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
