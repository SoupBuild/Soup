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
import Soup.Build.Extensions;
import SoupCompiler;
import SoupCompilerClang;
import SoupCompilerMSVC;

using namespace Opal;

#include "Tasks/TestBuildTask.h"

#define DllExport __declspec(dllexport)

std::shared_ptr<Soup::Compiler::ICompiler> CreateMSVCCompiler(Soup::Build::Extensions::ValueTableWrapper& activeState)
{
	auto visualCompilerToolsRoot = activeState.GetValue("MSVC.VCToolsBinRoot").AsString().GetValue();
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