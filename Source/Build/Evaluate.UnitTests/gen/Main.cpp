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
#include "BuildEvaluateEngineTests.gen.h"

#include "FileSystemState/FileSystemStateTests.gen.h"
#include "FileSystemState/FileSystemStateManagerTests.gen.h"
#include "FileSystemState/FileSystemStateReaderTests.gen.h"
#include "FileSystemState/FileSystemStateWriterTests.gen.h"

#include "OperationGraph/OperationGraphTests.gen.h"
#include "OperationGraph/OperationGraphManagerTests.gen.h"
#include "OperationGraph/OperationGraphReaderTests.gen.h"
#include "OperationGraph/OperationGraphWriterTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildHistoryCheckerTests();
	state += RunBuildEvaluateEngineTests();
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
