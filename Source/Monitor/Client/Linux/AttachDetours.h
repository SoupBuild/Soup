#pragma once

#include "Functions/Cache/FileApi.h"

namespace Monitor::Linux
{
	void AttachDetours()
	{
		Functions::Cache::FileApi::open = (open_ptr)dlsym(RTLD_NEXT, "open");

		Functions::Cache::FileApi::fopen = (fopen_ptr)dlsym(RTLD_NEXT, "fopen");
		Functions::Cache::FileApi::fdopen = (fdopen_ptr)dlsym(RTLD_NEXT, "fdopen");
		Functions::Cache::FileApi::freopen = (freopen_ptr)dlsym(RTLD_NEXT, "freopen");

		Functions::Cache::FileApi::mkdir = (mkdir_ptr)dlsym(RTLD_NEXT, "mkdir");
	}
}
