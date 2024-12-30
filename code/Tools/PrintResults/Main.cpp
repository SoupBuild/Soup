#include <iostream>
#include <filesystem>
#include <sstream>
#include <unordered_set>
#include <vector>

import Opal;
import Soup.Core;

void PrintUsage()
{
	std::cout << "printresults [path]" << std::endl;
}

std::string ToString(const std::vector<uint32_t>& valueList)
{
	auto builder = std::stringstream();
	builder << "[";
	bool isFirst = true;
	for (auto value : valueList)
	{
		if (!isFirst)
		{
			builder << ", ";
		}

		builder << value;
		isFirst = false;
	}

	builder << "]";
	return builder.str();
}

void PrintFiles(const Soup::Core::FileSystemState& fileSystemState)
{
	for (const auto& [key, value] : fileSystemState.GetFiles())
	{
		std::cout << "File: " << key << " " << value.ToString() << std::endl;
	}
}

std::string format(std::chrono::time_point<std::chrono::system_clock> time)
{
#ifdef _WIN32
	return std::format("{:%Y-%m-%d %H:%M:%S %z}", time);
#else
	auto timeT = std::chrono::system_clock::to_time_t(time);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S %z");

	return ss.str();
#endif
}

void PrintResults(Soup::Core::OperationResults& results)
{
	for (const auto& [key, operationResult] : results.GetResults())
	{
		#ifdef _WIN32
		auto evaluateTimeSystem = std::chrono::clock_cast<std::chrono::system_clock>(operationResult.EvaluateTime);
		#else
		auto evaluateTimeSystem = std::chrono::file_clock::to_sys(operationResult.EvaluateTime);
		#endif

		std::cout << "Operation: " << key << std::endl;
		std::cout << "\tEvaluateTime: " << format(evaluateTimeSystem) << std::endl;
		std::cout << "\tWasSuccessfulRun: " << operationResult.WasSuccessfulRun << std::endl;
		std::cout << "\tObservedInput: " << ToString(operationResult.ObservedInput) << std::endl;
		std::cout << "\tObservedOutput: " << ToString(operationResult.ObservedOutput) << std::endl;
	}
}

void LoadAndPrintResults(const Opal::Path& operationResultsFile)
{
	if (!Opal::System::IFileSystem::Current().Exists(operationResultsFile))
	{
		throw std::runtime_error("Operation results file does not exist");
	}

	// Open the file to read from
	auto file = Opal::System::IFileSystem::Current().OpenRead(operationResultsFile, true);

	// Read the contents of the build state file
	auto fileSystemState = Soup::Core::FileSystemState();
	auto results = Soup::Core::OperationResultsReader::Deserialize(file->GetInStream(), fileSystemState);

	PrintFiles(fileSystemState);
	PrintResults(results);
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

		auto operationResultsFile = Opal::Path::Parse(argv[1]);
		LoadAndPrintResults(operationResultsFile);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}