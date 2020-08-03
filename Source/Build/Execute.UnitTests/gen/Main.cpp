#include <any>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Soup.Build.Execute;
import json11;
import SoupTest;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;

#include "BuildHistoryCheckerTests.gen.h"
#include "BuildHistoryJsonTests.gen.h"
#include "BuildHistoryTests.gen.h"
#include "BuildHistoryManagerTests.gen.h"
#include "BuildRunnerTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildHistoryCheckerTests();
	state += RunBuildHistoryJsonTests();
	state += RunBuildHistoryTests();
	state += RunBuildHistoryManagerTests();
	state += RunBuildRunnerTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
