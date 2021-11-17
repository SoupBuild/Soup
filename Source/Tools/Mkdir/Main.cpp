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
				std::cout << "Directory already exists" << std::endl;
			else
				throw std::runtime_error("Create directory failed");
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