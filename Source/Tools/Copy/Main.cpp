#include <filesystem>
#include <iostream>
#include <string_view>

void PrintUsage()
{
	std::cout << "copy [source] [destination]" << std::endl;
}

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
		auto copyOptions = std::filesystem::copy_options::update_existing;
		std::filesystem::copy(sourcePath, destinationPath, copyOptions);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}