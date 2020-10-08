// <copyright file="RegisterBuildExtension.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <functional>
#include <map>
#include <stdexcept>
#include <string_view>
#include <unordered_set>

import Opal;
import Soup.Build;
import Soup.Build.Utilities;
import Soup.Cpp.Compiler;
import Soup.Cpp.Compiler.Clang;
import Soup.Cpp.Compiler.MSVC;

using namespace Opal;

#include "Tasks/TestBuildTask.h"

#define DllExport __declspec(dllexport)

std::shared_ptr<Soup::Cpp::Compiler::ICompiler> CreateMSVCCompiler(Soup::Build::Utilities::ValueTableWrapper& activeState)
{
	auto visualCompilerToolsRoot = activeState.GetValue("MSVC.VCToolsBinaryRoot").AsString().GetValue();
	std::shared_ptr<Soup::Cpp::Compiler::ICompiler> compiler = std::make_shared<Soup::Cpp::Compiler::MSVC::Compiler>(
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
		System::IProcessManager::Register(std::make_shared<System::WindowsProcessManager>());

		// Register all known compilers
		auto compilerFactory = Soup::Test::CompilerFactory();
		compilerFactory.emplace("MSVC", CreateMSVCCompiler);

		// Register the before build task
		auto testBuildTask = Opal::Memory::Reference<Soup::Test::TestBuildTask>(
			new Soup::Test::TestBuildTask(std::move(compilerFactory)));
		auto registerResult = buildSystem.TryRegisterTask(testBuildTask.GetRaw());
		if (registerResult != Soup::Build::ApiCallResult::Success)
			return -1;

		return 0;
	}
}