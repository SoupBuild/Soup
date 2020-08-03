#include <filesystem>
#include <iostream>
#include <string_view>

void PrintUsage()
{
	std::cout << "mkdir [path]" << std::endl;
}

void CreateDirectory(const std::filesystem::path& directory)
{
	std::error_code errorCode;
	if (!std::filesystem::create_directory(directory, errorCode))
	{
		if (!errorCode)
		{
			// The directory already existed
		}
		else if (errorCode == std::errc::no_such_file_or_directory)
		{
			// Create the parent recursively and then retry this folder
			auto parentPath = directory.parent_path();
			CreateDirectory(parentPath);
			std::filesystem::create_directory(directory);
		}
		else
		{
			throw std::system_error(errorCode);
		}
	}
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
		CreateDirectory(directory);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}