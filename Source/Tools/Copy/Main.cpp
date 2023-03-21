#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#else
#error "Unknown platform"
#endif
#include <iostream>
#include <string_view>
#include <string>

void PrintUsage()
{
	std::cout << "copy [source] [destination]" << std::endl;
}

#if defined(_WIN32)
void CopyFile(std::string_view sourcePath, std::string_view destinationPath)
{
	if (!CopyFileA(sourcePath.data(), destinationPath.data(), false))
	{
		throw std::runtime_error("Copy failed");
	}
}
#elif defined(__linux__)
void CopyFile(std::string_view sourcePath, std::string_view destinationPath)
{
}
#else
#error "Unknown platform"
#endif

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		PrintUsage();
		return 1;
	}

	try
	{
		auto sourcePath = std::string_view(argv[1]);
		auto destinationPath = std::string_view(argv[2]);
		if (!CopyFileA(sourcePath.data(), destinationPath.data(), false))
		{
			auto errorCode = GetLastError();
			if (errorCode == ERROR_FILE_NOT_FOUND)
			{
				throw std::runtime_error("File does not exist");
			}
			else
			{
				throw std::runtime_error("Copy failed: " + std::to_string(errorCode));
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}