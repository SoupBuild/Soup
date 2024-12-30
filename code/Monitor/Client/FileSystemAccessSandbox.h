
#pragma once
#include "Helpers.h"
#include "ConnectionManager.h"

namespace Monitor
{
	class FileSystemAccessSandbox
	{
	public:
		static void Initialize(
			bool enableAccessChecks,
			Opal::Path workingDirectory,
			std::vector<std::string> allowedReadDirectories,
			std::vector<std::string> allowedWriteDirectories)
		{
			connectionManager.DebugTrace("FileSystemAccessSandbox::Initialize");

			m_enableAccessChecks = enableAccessChecks;
			m_workingDirectory = std::move(workingDirectory);

			m_allowedReadDirectories.clear();
			for (auto& value : allowedReadDirectories)
				m_allowedReadDirectories.push_back(NormalizePath(value.c_str()));

			m_allowedWriteDirectories.clear();
			for (auto& value : allowedWriteDirectories)
				m_allowedWriteDirectories.push_back(NormalizePath(value.c_str()));
		}

		static void UpdateWorkingDirectory(const wchar_t* fileName)
		{
			auto fileNameEncoded = UTF8Encode(fileName);
			return UpdateWorkingDirectory(fileNameEncoded.c_str());
		}

		static void UpdateWorkingDirectory(const char* fileName)
		{
			auto path = Opal::Path::Parse(fileName);
			if (!path.HasRoot())
			{
				// Updated working directory is relative to the previous one
				m_workingDirectory = m_workingDirectory + path;
			}
			else
			{
				m_workingDirectory = std::move(path);
			}
		}

		static bool IsAllowed(const wchar_t* fileName, DWORD desiredAccess)
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
				return IsWriteAllowed(fileName);
			else
				return IsReadAllowed(fileName);
		}

		static bool IsAllowed(const char* fileName, DWORD desiredAccess)
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
				return IsWriteAllowed(fileName);
			else
				return IsReadAllowed(fileName);
		}

		static bool IsReadAllowed(const char* fileName)
		{
			if (!m_enableAccessChecks)
				return true;

			if (IsPipe(fileName))
				return true;

			auto normalizedFileName = NormalizePath(fileName);
			for (const auto& allowedDirectory : m_allowedReadDirectories)
			{
				if (IsUnderDirectory(normalizedFileName, allowedDirectory))
					return true;
			}

			return false;
		}

		static bool IsReadAllowed(const wchar_t* fileName)
		{
			if (!m_enableAccessChecks)
				return true;

			auto fileNameEncoded = UTF8Encode(fileName);
			return IsReadAllowed(fileNameEncoded.c_str());
		}

		static bool IsWriteAllowed(const char* fileName)
		{
			if (!m_enableAccessChecks)
				return true;

			if (IsPipe(fileName))
				return true;

			auto normalizedFileName = NormalizePath(fileName);
			for (const auto& allowedDirectory : m_allowedWriteDirectories)
			{
				if (IsUnderDirectory(normalizedFileName, allowedDirectory))
					return true;
			}

			return false;
		}

		static bool IsWriteAllowed(const wchar_t* fileName)
		{
			if (!m_enableAccessChecks)
				return true;

			auto fileNameEncoded = UTF8Encode(fileName);
			return IsWriteAllowed(fileNameEncoded.c_str());
		}

	private:
		static bool IsPipe(const char* fileName)
		{
			auto file = std::string(fileName);
			return file.starts_with("\\\\.\\");
		}

		static std::string NormalizePath(const char* fileName)
		{
			// Normalize the path separators and get absolute path
			auto path = Opal::Path::Parse(fileName);
			if (!path.HasRoot())
			{
				path = m_workingDirectory + path;
			}

			auto result = path.ToString();

			// Ignore case
			std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return (char)std::toupper(c); });

			return result;
		}

		static bool IsUnderDirectory(const std::string& fileName, const std::string& directory)
		{
			return fileName.find(directory) == 0;
		}

		static std::string UTF8Encode(const std::wstring_view wideString)
		{
			if (wideString.empty())
				return std::string();

			int requiredSizes = WideCharToMultiByte(
				CP_UTF8, 0, wideString.data(), (int)wideString.size(), nullptr, 0, nullptr, nullptr);

			std::string result(requiredSizes, 0);
			WideCharToMultiByte(
				CP_UTF8, 0, wideString.data(), (int)wideString.size(), result.data(), requiredSizes, nullptr, nullptr);

			return result;
		}

		static bool m_enableAccessChecks;
		static Opal::Path m_workingDirectory;

		static std::vector<std::string> m_allowedReadDirectories;
		static std::vector<std::string> m_allowedWriteDirectories;
	};

	bool FileSystemAccessSandbox::m_enableAccessChecks = false;
	Opal::Path FileSystemAccessSandbox::m_workingDirectory = Opal::Path();
	std::vector<std::string> FileSystemAccessSandbox::m_allowedReadDirectories = std::vector<std::string>();
	std::vector<std::string> FileSystemAccessSandbox::m_allowedWriteDirectories = std::vector<std::string>();
}
