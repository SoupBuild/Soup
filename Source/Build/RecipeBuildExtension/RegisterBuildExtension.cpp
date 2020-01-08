module;

#include <any>
#include <array>
#include <ctime>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define DllExport __declspec( dllexport )

module RecipeBuildExtension;
using namespace Opal;

extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Setup the filter
		auto defaultTypes =
			static_cast<uint32_t>(Soup::TraceEventFlag::Warning) |
			static_cast<uint32_t>(Soup::TraceEventFlag::Error) |
			static_cast<uint32_t>(Soup::TraceEventFlag::Critical);
		auto filter = std::make_shared<Soup::EventTypeFilter>(
				static_cast<Soup::TraceEventFlag>(defaultTypes));

		// Setup the console listener
		Log::RegisterListener(
			std::make_shared<Soup::ConsoleTraceListener>(
				"Log",
				filter,
				false,
				false));

		// Setup the real services
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
		System::IProcessManager::Register(std::make_shared<System::PlatformProcessManager>());

		// Register the resolve tools task
		auto resolveToolsTask = Memory::Reference<RecipeBuild::ResolveToolsTask>(
			new RecipeBuild::ResolveToolsTask());
		buildSystem.RegisterTask(resolveToolsTask.GetRaw());

		// Register the standard library include task
		auto standardLibraryIncludeTask = Memory::Reference<RecipeBuild::StandardLibraryIncludeTask>(
			new RecipeBuild::StandardLibraryIncludeTask());
		buildSystem.RegisterTask(standardLibraryIncludeTask.GetRaw());

		// Register the recipe build task
		auto recipeBuildTask = Memory::Reference<RecipeBuild::RecipeBuildTask>(
			new RecipeBuild::RecipeBuildTask());
		buildSystem.RegisterTask(recipeBuildTask.GetRaw());

		// Register the compile task
		auto buildTask = Memory::Reference<RecipeBuild::BuildTask>(
			new RecipeBuild::BuildTask());
		buildSystem.RegisterTask(buildTask.GetRaw());

		return 0;
	}
}