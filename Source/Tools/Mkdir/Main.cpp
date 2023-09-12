#include <chrono>
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
			auto now = std::chrono::system_clock::now();
			#ifdef _WIN32
			auto nowFileTime = std::chrono::clock_cast<std::chrono::file_clock>(now);
			#else
			auto nowFileTime = std::chrono::file_clock::from_sys(now);
			#endif
			fileSystem.SetLastWriteTime(directory, nowFileTime);
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