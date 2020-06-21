import Detours;

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <Windows.h>
#include "DetouredProcess.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << argv[0] << " <pathToTool> [<arguments>]" << std::endl;
		return 1;
	}

	auto tool = std::string(argv[1]);
	std::vector<std::string> arguments;
	for (int i = 2; i < argc; i++)
	{
		arguments.push_back(argv[i]);
	}

	auto process = DetouredProcess();
	process.RunProcess(tool, arguments);

	// var accessesByPath = result
	// 	.FileAccesses
	// 	.Select(access => access.GetPath(fileAccessReporter.PathTable))
	// 	.Distinct(StringComparer.OrdinalIgnoreCase);

	// var displayAccesses = string.Join(Environment.NewLine, accessesByPath);

	// Console.WriteLine($"Process '{tool}' ran under BuildXL sandbox with arguments '{arguments}' and returned with exit code '{result.ExitCode}'. Sandbox reports {result.FileAccesses.Count} file accesses:");
	// Console.WriteLine(displayAccesses);

	return 0;
}