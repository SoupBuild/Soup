// Include all standard library headers in the global module
#include <filesystem>
#include <memory>
#include <string>

#ifdef _WIN32
#include <combaseapi.h>
#define SOUP_TOOLS_API __declspec(dllexport)
#else
#define SOUP_TOOLS_API
#define CoTaskMemAlloc(p) malloc(p)
#define CoTaskMemFree(p) free(p)
#endif

import json11;
import Opal;
import Soup.Core;

using namespace Opal;
using namespace Soup::Core;

std::string LoadBuildGraphContent(const Path& workingDirectory)
{
	try
	{
		// Setup the filter
		auto defaultTypes =
			static_cast<uint32_t>(TraceEventFlag::HighPriority) |
			static_cast<uint32_t>(TraceEventFlag::Warning) |
			static_cast<uint32_t>(TraceEventFlag::Error) |
			static_cast<uint32_t>(TraceEventFlag::Critical);
		auto filter = std::make_shared<EventTypeFilter>(
				static_cast<TraceEventFlag>(defaultTypes));

		// Setup the console listener
		Log::RegisterListener(
			std::make_shared<ConsoleTraceListener>(
				"Log",
				filter,
				false,
				false));

		// Setup the real services
		System::ISystem::Register(std::make_shared<System::STLSystem>());
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());

		auto globalParameters = ValueTable();

		// Load user config state
		auto builtInPackageDirectory = Path("C:/Program Files/SoupBuild/Soup/Soup/BuiltIn/");
		auto userDataPath = BuildEngine::GetSoupUserDataPath();
		
		auto recipeCache = RecipeCache();

		auto packageProvider = BuildEngine::LoadBuildGraph(
			builtInPackageDirectory,
			workingDirectory,
			globalParameters,
			userDataPath,
			recipeCache);

		auto keys = json11::Json::array({ 1, 2, 3 });

		json11::Json jsonResult = json11::Json::object({
			{ "Message", "" },
			{ "IsSuccess", true },
			{ "Keys", std::move(keys) },
		});
		auto value = jsonResult.dump();
		return value;
	}
	catch (const HandledException&)
	{
		json11::Json jsonResult = json11::Json::object({
			{ "Message", "FAILED" },
			{ "IsSuccess", false },
		});
		return jsonResult.dump();
	}
	catch(const std::exception& e)
	{
		json11::Json jsonResult = json11::Json::object({
			{ "Message", e.what() },
			{ "IsSuccess", false },
		});
		return jsonResult.dump();
	}
}

extern "C"
{
	SOUP_TOOLS_API const char* LoadBuildGraph(const char* workingDirectory)
	{
		auto workingDirectoryPath = Path(workingDirectory);
		auto value = LoadBuildGraphContent(workingDirectoryPath);

		auto result = (char*)CoTaskMemAlloc(value.size() + 1);
		value.copy(result, value.size());
		result[value.size()] = 0;
		return result;
	}
	
	SOUP_TOOLS_API int AddStuff(int a, int b)
	{
		return a + b;
	}
}