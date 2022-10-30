#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

import Opal;
import Soup.Core;

void PrintUsage()
{
	std::cout << "printgraph [path]" << std::endl;
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
	for (auto fileReference : fileSystemState.GetFiles())
	{
		std::cout << "File: " << fileReference.first << " " << fileReference.second.ToString() << std::endl;
	}
}

void PrintOperations(Soup::Core::OperationGraph& graph)
{
	for (auto operation : graph.GetOperations())
	{
		const auto& operationInfo = operation.second;
		std::cout << "Operation: " << operationInfo.Id << std::endl;
		std::cout << "\tTitle: " << operationInfo.Title << std::endl;
		std::cout << "\tCommand-WorkingDirectory: " << operationInfo.Command.WorkingDirectory.ToString() << std::endl;
		std::cout << "\tCommand-Executable: " << operationInfo.Command.Executable.ToString() << std::endl;
		std::cout << "\tCommand-Arguments: " << operationInfo.Command.Arguments << std::endl;
		std::cout << "\tDeclaredInput: " << ToString(operationInfo.DeclaredInput) << std::endl;
		std::cout << "\tDeclaredOutput: " << ToString(operationInfo.DeclaredOutput) << std::endl;
		std::cout << "\tReadAccess: " << ToString(operationInfo.ReadAccess) << std::endl;
		std::cout << "\tWriteAccess: " << ToString(operationInfo.WriteAccess) << std::endl;
		std::cout << "\tChildren: " << ToString(operationInfo.Children) << std::endl;
		std::cout << "\tDependencyCount: " << operationInfo.DependencyCount << std::endl;
	}
}

void PrintGraph(
	const std::string& indent,
	const std::vector<Soup::Core::OperationId>& operationIds,
	Soup::Core::OperationGraph& graph,
	const std::unordered_set<Soup::Core::OperationId>& parentOperationSet)
{
	auto childIndent = indent + "  ";
	for (auto operationId : operationIds)
	{
		const auto& operationInfo = graph.GetOperationInfo(operationId);
		if (parentOperationSet.find(operationId) != parentOperationSet.end())
		{
			std::cerr << indent << "ERROR: CIRCULAR DEPENDENCIES -" << operationInfo.Title << std::endl;
		}
		else
		{
			std::cout << indent << operationInfo.Title << std::endl;
			auto updatedParentSet = std::unordered_set<Soup::Core::OperationId>(parentOperationSet);
			updatedParentSet.insert(operationId);
			PrintGraph(childIndent, operationInfo.Children, graph, updatedParentSet);
		}
	}
}

void PrintGraph(Soup::Core::OperationGraph& graph)
{
	std::cout << "Graph:" << std::endl;
	auto emptySet = std::unordered_set<Soup::Core::OperationId>();
	PrintGraph("", graph.GetRootOperationIds(), graph, emptySet);
}

void LoadAndPrintGraph(const Opal::Path& operationGraphFile)
{
	if (!Opal::System::IFileSystem::Current().Exists(operationGraphFile))
	{
		throw std::runtime_error("Operation graph file does not exist");
	}

	// Open the file to read from
	auto file = Opal::System::IFileSystem::Current().OpenRead(operationGraphFile, true);

	// Read the contents of the build state file
	auto fileSystemState = Soup::Core::FileSystemState();
	auto graph = Soup::Core::OperationGraphReader::Deserialize(file->GetInStream(), fileSystemState);

	PrintFiles(fileSystemState);
	PrintOperations(graph);
	PrintGraph(graph);
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

		auto operationGraphFile = Opal::Path(argv[1]);
		LoadAndPrintGraph(operationGraphFile);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 2;
	}

	return 0;
}