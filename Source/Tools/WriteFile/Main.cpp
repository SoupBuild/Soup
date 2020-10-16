#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

void PrintUsage()
{
	std::cout << "writefile [filename] [content]" << std::endl;
}

// TODO: Should not be doing my own raw UTF-8 manipulation, find a library or create one in Opal
void UnescapeString(std::string_view value, std::ostream& stream)
{
	for (auto character = value.cbegin(); character != value.cend(); character++)
	{
		if (*character == '\\')
		{
			character++;
			switch (*character)
			{
			case '"':
				stream << "\"";
				break;
			case '\\':
				stream << "\\";
				break;
			case 'b':
				stream << "\b";
				break;
			case 'f':
				stream << "\f";
				break;
			case 'n':
				stream << "\n";
				break;
			case 'r':
				stream << "\r";
				break;
			case 't':
				stream << "\t";
				break;
			case 'u':
				throw std::runtime_error("Unicode not supported yet...");
			default:
				throw std::runtime_error("Unknown escape character");
			}
		}
		else
		{
			stream << *character;
		}
	}
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

		auto fileStream = std::ofstream(destinationPath);
		UnescapeString(content, fileStream);
		fileStream.close();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}