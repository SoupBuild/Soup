#if defined(_WIN32)
#include <Windows.h>
#include <Shlobj.h>
#undef GetCurrentDirectory
#elif defined(__linux__)
#include <sys/stat.h> 
#else
#error "Unknown platform"
#endif
#include <iostream>
#include <filesystem>

import Opal;

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
		auto directory = Opal::Path(argv[1]);
		auto fileSystem = Opal::System::STLFileSystem();

		if (fileSystem.Exists(directory))
		{
			auto now = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::system_clock::now());
			fileSystem.SetLastWriteTime(directory, now);
		}
		else
		{
			fileSystem.CreateDirectory2(directory);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}