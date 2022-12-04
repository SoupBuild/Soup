#include <iostream>
#include <vector>

import Opal;
import Soup.Core;

void PrintUsage()
{
	std::cout << "printvaluetable [path]" << std::endl;
}

void PrintValue(Soup::Core::Value& value, const std::string& indent);

void PrintValueTable(Soup::Core::ValueTable& values, const std::string& indent)
{
	std::cout << "{" << std::endl;
	auto nextIndent = indent + "  ";
	for (auto tableValue : values)
	{
		// Write the key
		std::cout << nextIndent << tableValue.first << ": ";

		// Write the value
		PrintValue(tableValue.second, nextIndent);

		std::cout << "," << std::endl;
	}

	std::cout << indent << "}";
}

void PrintValueList(const Soup::Core::ValueList& values, const std::string& indent)
{
	std::cout << "[" << std::endl;
	auto nextIndent = indent + "  ";
	for (auto value : values)
	{
		std::cout << nextIndent;
		PrintValue(value, nextIndent);

		std::cout << "," << std::endl;
	}

	std::cout << indent << "]";
}

void PrintValue(Soup::Core::Value& value, const std::string& indent)
{
	auto valueType = value.GetType();
	switch (valueType)
	{
	case Soup::Core::ValueType::Table:
		PrintValueTable(value.AsTable(), indent);
		break;
	case Soup::Core::ValueType::List:
		PrintValueList(value.AsList(), indent);
		break;
	case Soup::Core::ValueType::String:
		std::cout << "\"" << value.AsString() << "\"";
		break;
	case Soup::Core::ValueType::Integer:
		std::cout << value.AsInteger();
		break;
	case Soup::Core::ValueType::Float:
		std::cout << value.AsFloat();
		break;
	case Soup::Core::ValueType::Boolean:
		std::cout << value.AsBoolean();
		break;
	default:
		throw std::runtime_error("Unknown ValueType");
	}
}

void LoadAndPrint(const Opal::Path& valueTableFile)
{
	if (!Opal::System::IFileSystem::Current().Exists(valueTableFile))
	{
		throw std::runtime_error("Value Table file does not exist");
	}

	// Open the file to read from
	auto file = Opal::System::IFileSystem::Current().OpenRead(valueTableFile, true);

	// Read the contents of the build state file
	auto valueTable = Soup::Core::ValueTableReader::Deserialize(file->GetInStream());

	PrintValueTable(valueTable, "");
	std::cout << std::endl;
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
		Opal::System::IFileSystem::Register(std::make_shared<Opal::System::STLFileSystem>());

		auto valueTableFile = Opal::Path(argv[1]);
		LoadAndPrint(valueTableFile);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}