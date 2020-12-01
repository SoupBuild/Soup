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
import Soup.Build.Utilities;
import Soup.Build.Runtime;
import Soup.Rust;
import Soup.Rust.Compiler;
import Soup.Test.Assert;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

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
