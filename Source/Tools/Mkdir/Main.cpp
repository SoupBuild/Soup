#include <Windows.h>
#include <iostream>

void PrintUsage()
{
	std::cout << "mkdir [path]" << std::endl;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		PrintUsage();
		return 1;
	}

	try
	{
		auto directory = std::string_view(argv[1]);
		if (!CreateDirectoryA(directory.data(), nullptr))
		{
			auto errorCode =  GetLastError();
			if (errorCode == ERROR_ALREADY_EXISTS)
			{
				std::cout << "Directory already exists" << std::endl;

				// Get the current system time
				SYSTEMTIME systemTime;
				GetSystemTime(&systemTime);
				
				// Convert time file time
				FILETIME fileTime;
				if (!SystemTimeToFileTime(&systemTime, &fileTime))
				{
					throw std::runtime_error("Failed to convert from system time to file time");
				}

				// Open the directory handle to write
				auto directoryHandle = CreateFileA(
					directory.data(),
					GENERIC_WRITE,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					nullptr,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS,
					0);
				if (directoryHandle == INVALID_HANDLE_VALUE)
				{
					auto createErrorCode =  GetLastError();
					std::cout << createErrorCode << std::endl;
					throw std::runtime_error("Failed to open directory handle to write");
				}

				// Mark the directory as updated
				if (!SetFileTime(directoryHandle, nullptr, nullptr, &fileTime))
				{
					throw std::runtime_error("Set directory time failed");
				}

				CloseHandle(directoryHandle);
			}
			else
			{
				throw std::runtime_error("Create directory failed");
			}
		}
		else
		{
			std::cout << "Directory created" << std::endl;
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}