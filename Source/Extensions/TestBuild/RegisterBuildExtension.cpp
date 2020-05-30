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

extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Register the before build task
		auto testBuildTask = Opal::Memory::Reference<Soup::Test::TestBuildTask>(
			new Soup::Test::TestBuildTask());
		buildSystem.RegisterTask(testBuildTask.GetRaw());

		return 0;
	}
}