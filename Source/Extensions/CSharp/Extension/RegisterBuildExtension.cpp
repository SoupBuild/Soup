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

module Soup.CSharp;

using namespace Opal;


extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Setup the real services
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
		System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());

		// Register the resolve tools task
		auto resolveToolsTask = Memory::Reference<Soup::CSharp::ResolveToolsTask>(
			new Soup::CSharp::ResolveToolsTask());
		if (buildSystem.TryRegisterTask(resolveToolsTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the standard library include task
		auto standardLibraryIncludeTask = Memory::Reference<Soup::CSharp::StandardLibraryIncludeTask>(
			new Soup::CSharp::StandardLibraryIncludeTask());
		if (buildSystem.TryRegisterTask(standardLibraryIncludeTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the standard library include task
		auto resolveDependenciesTask = Memory::Reference<Soup::CSharp::ResolveDependenciesTask>(
			new Soup::CSharp::ResolveDependenciesTask());
		if (buildSystem.TryRegisterTask(resolveDependenciesTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the recipe build task
		auto recipeBuildTask = Memory::Reference<Soup::CSharp::RecipeBuildTask>(
			new Soup::CSharp::RecipeBuildTask());
		if (buildSystem.TryRegisterTask(recipeBuildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the compile task
		auto buildTask = Memory::Reference<Soup::CSharp::BuildTask>(
			new Soup::CSharp::BuildTask());
		if (buildSystem.TryRegisterTask(buildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		return 0;
	}
}