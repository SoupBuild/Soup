
#pragma once
#include "Helpers.h"
#include "ConnectionManager.h"

namespace Monitor
{
	class FileSystemAccessSandbox
	{
	public:
		static void Initialize()
		{
			ConnectionManager::DebugTrace("FileSystemAccessSandbox::Initialize");
			m_allowedReadDirectories.push_back(
				NormalizePath("C:\\Windows\\"));
			m_allowedReadDirectories.push_back(
				NormalizePath("C:/Program Files (x86)/Microsoft Visual Studio/Installer/"));
			m_allowedReadDirectories.push_back(
				NormalizePath("C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\MSBuild\\Current\\Bin\\Roslyn\\"));
			m_allowedReadDirectories.push_back(
				NormalizePath("C:/Program Files/dotnet/"));
			m_allowedReadDirectories.push_back(
				NormalizePath("C:\\Users\\mwasp\\source\\repos\\Soup\\Source\\GenerateSharp\\Opal\\"));
			m_allowedReadDirectories.push_back(
				NormalizePath("C:/Users/mwasp/source/repos/Soup/out/C#/Opal/d6cd8e54c2437463cfea37201849278a10a3fb82df1565fbc685206e25f5/"));

			m_allowedWriteDirectories.push_back(
				NormalizePath("C:/Users/mwasp/source/repos/Soup/out/C#/Opal/d6cd8e54c2437463cfea37201849278a10a3fb82df1565fbc685206e25f5/"));
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
			auto normalizedFileName = NormalizePath(fileName);
			for (const auto& allowedDiractory : m_allowedReadDirectories)
			{
				if (IsUnderDirectory(normalizedFileName, allowedDiractory))
					return true;
			}

			Monitor::ConnectionManager::DebugError("Prevent Read user");
			return false;
		}

		static bool IsReadAllowed(const wchar_t* fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			return IsReadAllowed(converter.to_bytes(fileName).c_str());
		}

		static bool IsWriteAllowed(const char* fileName)
		{
			auto normalizedFileName = NormalizePath(fileName);
			for (const auto& allowedDiractory : m_allowedWriteDirectories)
			{
				if (IsUnderDirectory(normalizedFileName, allowedDiractory))
					return true;
			}

			Monitor::ConnectionManager::DebugError("Prevent Write user");
			return false;
		}

		static bool IsWriteAllowed(const wchar_t* fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			return IsWriteAllowed(converter.to_bytes(fileName).c_str());
		}

	private:
		static std::string NormalizePath(const char* fileName)
		{
			// Normalize the path separators
			auto path = Opal::Path(fileName);
			auto normalizedFileName = path.ToString();

			// Normalize to uppercase
			// TODO: May want to use wide characters to perform this to upper in a safe way
			std::transform(
				normalizedFileName.begin(),
				normalizedFileName.end(),
				normalizedFileName.begin(),
				[](unsigned char c) -> unsigned char { return (unsigned char)std::toupper(c); });

			return normalizedFileName;
		}

		static bool IsUnderDirectory(const std::string& fileName, const std::string& directory)
		{
			// Check if the fileName starts with the directory
			return fileName.find(directory) == 0;
		}

		static std::vector<std::string> m_allowedReadDirectories;
		static std::vector<std::string> m_allowedWriteDirectories;
	};

	std::vector<std::string> FileSystemAccessSandbox::m_allowedReadDirectories = std::vector<std::string>();
	std::vector<std::string> FileSystemAccessSandbox::m_allowedWriteDirectories = std::vector<std::string>();
}
