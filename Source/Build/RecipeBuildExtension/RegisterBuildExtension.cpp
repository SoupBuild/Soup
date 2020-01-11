module;

#include <any>
#include <array>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
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

std::shared_ptr<Soup::ICompiler> CreateMSVCCompiler(Soup::Build::PropertyBagWrapper& activeState)
{
	auto visualCompilerToolsRoot = activeState.GetPropertyStringValue("MSVC.VCToolsRoot");
	std::shared_ptr<Soup::ICompiler> compiler = std::make_shared<Soup::Compiler::MSVC::Compiler>(
		Path(visualCompilerToolsRoot) + Path("bin/Hostx64/x64/"),
		Path("cl.exe"),
		Path("link.exe"),
		Path("lib.exe"));

	return compiler;
}

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

		// Register all known compilers
		auto compilerFactory = RecipeBuild::CompilerFactory();
		compilerFactory.emplace("MSVC", CreateMSVCCompiler);

		// Register the compile task
		auto buildTask = Memory::Reference<RecipeBuild::BuildTask>(
			new RecipeBuild::BuildTask(std::move(compilerFactory)));
		buildSystem.RegisterTask(buildTask.GetRaw());

		return 0;
	}
}