import Monitor.Shared;
import Opal;

using namespace Opal;

#include <codecvt>
#include <locale>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

int main(int argc, char **argv)
{
	try
	{
		if (argc <= 1)
		{
			std::cout << "Missing executable parameter." << std::endl;
			return 1;
		}

		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
		Monitor::IDetourProcessManager::Register(std::make_shared<Monitor::WindowsDetourProcessManager>());
		System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());

		auto application = Path(argv[1]);
		std::stringstream argumentsValue;
		for (int i = 2; i < argc; i++)
		{
			argumentsValue << " " << argv[i];
		}

		std::string argumentsString = argumentsValue.str();
		auto workingDirectory = Path();

		auto file = std::fstream("Access.txt", std::fstream::out);

		auto callback = std::make_shared<Monitor::DetourCallbackLogger>(file);
		auto process = Monitor::IDetourProcessManager::Current().CreateDetourProcess(
			application,
			argumentsString,
			workingDirectory,
			callback);
		process->Start();
		process->WaitForExit();
		auto result = process->GetExitCode();

		file.close();

		return result;
	}
	catch(const std::exception& ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
		return -1;
	}
}
