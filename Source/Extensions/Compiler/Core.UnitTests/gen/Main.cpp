#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Soup.Build.Evaluate;
import SoupCompiler;
import SoupTest;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;

#include "BuildEngineTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildEngineTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
