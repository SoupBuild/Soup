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

module Soup.Rust;

using namespace Opal;


extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Setup the real services
		System::IFileSystem::Register(std::make_shared<System::STLFileSystem>());
		System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());

		// Register the resolve tools task
		auto resolveToolsTask = Memory::Reference<Soup::Rust::ResolveToolsTask>(
			new Soup::Rust::ResolveToolsTask());
		if (buildSystem.TryRegisterTask(resolveToolsTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the standard library include task
		auto resolveDependenciesTask = Memory::Reference<Soup::Rust::ResolveDependenciesTask>(
			new Soup::Rust::ResolveDependenciesTask());
		if (buildSystem.TryRegisterTask(resolveDependenciesTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the recipe build task
		auto recipeBuildTask = Memory::Reference<Soup::Rust::RecipeBuildTask>(
			new Soup::Rust::RecipeBuildTask());
		if (buildSystem.TryRegisterTask(recipeBuildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the compile task
		auto buildTask = Memory::Reference<Soup::Rust::BuildTask>(
			new Soup::Rust::BuildTask());
		if (buildSystem.TryRegisterTask(buildTask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		return 0;
	}
}