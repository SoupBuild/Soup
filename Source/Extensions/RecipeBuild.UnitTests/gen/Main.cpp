#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import SoupCore;
import RecipeBuildExtension;
import json11;
import SoupTest;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;
import SoupTestUtilities;

#include "Tasks/BuildTaskTests.gen.h"
#include "Tasks/RecipeBuildTaskTests.gen.h"
#include "Tasks/ResolveToolsTaskTests.gen.h"
#include "Tasks/StandardLibraryIncludeTaskTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildTaskTests();
	state += RunRecipeBuildTaskTests();
	state += RunResolveToolsTaskTests();
	state += RunStandardLibraryIncludeTaskTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
