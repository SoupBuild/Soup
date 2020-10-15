#include <filesystem>
#include <iostream>
#include <string_view>

void PrintUsage()
{
	std::cout << "writefile [filename] [content]" << std::endl;
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
		auto destinationPath = std::string_view(argv[1]);
		auto content = std::string_view(argv[2]);

		auto file = std::ofstream(destinationPath);
		file << content;
		file.close();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}