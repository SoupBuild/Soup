#include <string_view>

import Opal;
import Soup.Build;
import Soup.Build.Utilities;

#include "AfterBuildTask.h"
#include "BeforeBuildTask.h"

#define DllExport __declspec(dllexport)
extern "C"
{
	DllExport int RegisterBuildExtension(Soup::Build::IBuildSystem& buildSystem)
	{
		// Register the before build task
		auto beforeBuildtask = Opal::Memory::Reference<SimpleBuildExtension::BeforeBuildTask>(
			new SimpleBuildExtension::BeforeBuildTask());
		if (buildSystem.TryRegisterTask(beforeBuildtask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		// Register the after build task
		auto afterBuildtask = Opal::Memory::Reference<SimpleBuildExtension::AfterBuildTask>(
			new SimpleBuildExtension::AfterBuildTask());
		if (buildSystem.TryRegisterTask(afterBuildtask.GetRaw()) != Soup::Build::ApiCallResult::Success)
			return -1;

		return 0;
	}
}