
#pragma once

class EventLogger
{
public:
	EventLogger() :
		m_pipeMutex(),
		m_pipeHandle(INVALID_HANDLE_VALUE)
	{
	}

	void Initialize(std::string_view pipeName)
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
			UnsafeWriteMessage(Monitor::DetourMessageType::Exit, "");
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
		std::stringstream contentBuilder;
		contentBuilder << existingFileName << "&" << newFileName;
		auto content = contentBuilder.str();

		WriteMessage(Monitor::DetourMessageType::CopyFile, content);
	}

	void LogCreateDirectory(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateDirectory(shortFileName);
	}

	void LogCreateDirectory(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::CreateDirectory, fileName);
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
		std::stringstream contentBuilder;
		contentBuilder << fileName << "&" << existingFileName;
		auto content = contentBuilder.str();

		WriteMessage(Monitor::DetourMessageType::CreateHardLink, content);
	}

	void LogCreateFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateFile(shortFileName);
	}

	void LogCreateFile(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::CreateFile, fileName);
	}

	void LogCreateProcess(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogCreateProcess(shortFileName);
	}

	void LogCreateProcess(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::CreateProcess, fileName);
	}

	void LogDeleteFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogDeleteFile(shortFileName);
	}

	void LogDeleteFile(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::DeleteFile, fileName);
	}

	void LogGetEnvironmentVariable(std::wstring_view name)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortName = converter.to_bytes(name.data());
		LogGetEnvironmentVariable(shortName);
	}

	void LogGetEnvironmentVariable(std::string_view name)
	{
		WriteMessage(Monitor::DetourMessageType::GetEnvironmentVariable, name);
	}

	void LogGetFileAttributes(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogGetFileAttributes(shortFileName);
	}

	void LogGetFileAttributes(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::GetFileAttributes, fileName);
	}

	void LogLoadLibrary(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogLoadLibrary(shortFileName);
	}

	void LogLoadLibrary(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::LoadLibrary, fileName);
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
		std::stringstream contentBuilder;
		contentBuilder << existingFileName << "&" << newFileName;
		auto content = contentBuilder.str();

		WriteMessage(Monitor::DetourMessageType::MoveFile, content);
	}

	void LogOpenFile(std::wstring_view fileName)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		auto shortFileName = converter.to_bytes(fileName.data());
		LogOpenFile(shortFileName);
	}

	void LogOpenFile(std::string_view fileName)
	{
		WriteMessage(Monitor::DetourMessageType::OpenFile, fileName);
	}

	void LogError(std::string_view message)
	{
		WriteMessage(Monitor::DetourMessageType::Error, message);
	}

private:
	void WriteMessage(Monitor::DetourMessageType type, std::string_view content)
	{
		auto lock = std::lock_guard<std::mutex>(m_pipeMutex);
		UnsafeWriteMessage(type, content);
	}

	void UnsafeWriteMessage(Monitor::DetourMessageType type, std::string_view content)
	{
		if (m_pipeHandle == INVALID_HANDLE_VALUE)
			return; // TODO: A static dll init may do bad things before the main entry initialize

		// Include null terminator in the length
		auto size = content.size() + 1;
		if (size > sizeof(Monitor::DetourMessage::Content))
			throw std::runtime_error("Message content too long");

		Monitor::DetourMessage message;
		message.Type = type;
		message.ContentSize = size + sizeof(Monitor::DetourMessage::Type) + sizeof(Monitor::DetourMessage::ContentSize);
		std::copy(content.begin(), content.end(), message.Content);
		message.Content[content.size()] = 0;

		// Write the message
		DWORD countBytesWritten = 0;
		if (!Functions::Cache::WriteFile(
			m_pipeHandle,
			&message,
			message.ContentSize,
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
