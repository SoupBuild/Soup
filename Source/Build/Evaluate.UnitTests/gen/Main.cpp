#include <any>
#include <array>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

import Opal;
import Soup.Build.Evaluate;
import Monitor.Shared;
import json11;
import Soup.Test.Assert;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

#include "BuildHistoryCheckerTests.gen.h"
#include "BuildRunnerTests.gen.h"
#include "FileSystemStateTests.gen.h"
#include "FileSystemStateManagerTests.gen.h"
#include "FileSystemStateReaderTests.gen.h"
#include "FileSystemStateWriterTests.gen.h"
#include "OperationGraphTests.gen.h"
#include "OperationGraphManagerTests.gen.h"
#include "OperationGraphReaderTests.gen.h"
#include "OperationGraphWriterTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildHistoryCheckerTests();
	state += RunBuildRunnerTests();
	state += RunFileSystemStateTests();
	state += RunFileSystemStateManagerTests();
	state += RunFileSystemStateReaderTests();
	state += RunFileSystemStateWriterTests();
	state += RunOperationGraphTests();
	state += RunOperationGraphManagerTests();
	state += RunOperationGraphReaderTests();
	state += RunOperationGraphWriterTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
