// <copyright file="RegisterBuildExtension.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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

module RecipeBuild;

using namespace Opal;

std::shared_ptr<Soup::Compiler::ICompiler> CreateMSVCCompiler(Soup::Build::Utilities::ValueTableWrapper& activeState)
{
	auto visualCompilerToolsRoot = activeState.GetValue("MSVC.VCToolsBinaryRoot").AsString().GetValue();
	std::shared_ptr<Soup::Compiler::ICompiler> compiler = std::make_shared<Soup::Compiler::MSVC::Compiler>(
		Path(visualCompilerToolsRoot),
		Path("cl.exe"),
		Path("link.exe"),
		Path("lib.exe"));

	return compiler;
}

extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Setup the real services
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
		System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());

		// Register the resolve tools task
		auto resolveToolsTask = Memory::Reference<RecipeBuild::ResolveToolsTask>(
			new RecipeBuild::ResolveToolsTask());
		if (buildSystem.TryRegisterTask(resolveToolsTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the standard library include task
		auto standardLibraryIncludeTask = Memory::Reference<RecipeBuild::StandardLibraryIncludeTask>(
			new RecipeBuild::StandardLibraryIncludeTask());
		if (buildSystem.TryRegisterTask(standardLibraryIncludeTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the standard library include task
		auto resolveDependenciesTask = Memory::Reference<RecipeBuild::ResolveDependenciesTask>(
			new RecipeBuild::ResolveDependenciesTask());
		if (buildSystem.TryRegisterTask(resolveDependenciesTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the recipe build task
		auto recipeBuildTask = Memory::Reference<RecipeBuild::RecipeBuildTask>(
			new RecipeBuild::RecipeBuildTask());
		if (buildSystem.TryRegisterTask(recipeBuildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register all known compilers
		auto compilerFactory = RecipeBuild::CompilerFactory();
		compilerFactory.emplace("MSVC", CreateMSVCCompiler);

		// Register the compile task
		auto buildTask = Memory::Reference<RecipeBuild::BuildTask>(
			new RecipeBuild::BuildTask(std::move(compilerFactory)));
		if (buildSystem.TryRegisterTask(buildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		return 0;
	}
}