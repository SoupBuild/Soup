#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <sys/stat.h> 
#else
#error "Unknown platform"
#endif
#include <iostream>

void PrintUsage()
{
	std::cout << "mkdir [path]" << std::endl;
}

#if defined(_WIN32)
	void CreateDirectory(std::string_view directory)
	{
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
#elif defined(__linux__)
	void CreateDirectory(std::string_view directory)
	{
		mode_t mode = 0755; 
		auto status = mkdir(directory.data(), mode);
		if (status != 0)
		{
			switch (errno)
			{
				case EACCES:
					// Search permission is denied on a component of the path prefix, or write permission is denied on the parent directory of the directory to be created.
					throw std::runtime_error("Create directory failed: Permission Denied");
				case EEXIST:
					// The named file exists.
					throw std::runtime_error("Create directory failed: Already Exists");
				case ELOOP:
					// A loop exists in symbolic links encountered during resolution of the path argument.
					throw std::runtime_error("Create directory failed: Loop Exists");
				case EMLINK:
					// The link count of the parent directory would exceed {LINK_MAX}.
					throw std::runtime_error("Create directory failed: Max Link");
				case ENAMETOOLONG:
					// The length of the path argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}.
					throw std::runtime_error("Create directory failed: Name Too Long");
				case ENOENT:
					// A component of the path prefix specified by path does not name an existing directory or path is an empty string.
					throw std::runtime_error("Create directory failed: Perfix Does Not Exist");
				case ENOSPC:
					// The file system does not contain enough space to hold the contents of the new directory or to extend the parent directory of the new directory.
					throw std::runtime_error("Create directory failed: No Space");
				case ENOTDIR:
					// A component of the path prefix is not a directory.
					throw std::runtime_error("Create directory failed: Not Directory");
				case EROFS:
					// The parent directory resides on a read-only file system.
					throw std::runtime_error("Create directory failed: Read Only");
				default:
					throw std::runtime_error("Create directory failed: Unknown Reason - " + std::to_string(status));
			}
		}
	}
#else
	#error "Unknown platform"
#endif

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
		CreateDirectory(directory);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}