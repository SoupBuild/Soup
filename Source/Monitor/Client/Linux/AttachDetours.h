#pragma once

#include "Functions/Cache/FileApi.h"

namespace Monitor::Linux
{
	void AttachDetours()
	{
		Functions::Cache::FileApi::open = (open_ptr)dlsym(RTLD_NEXT, "open");
	}
}
