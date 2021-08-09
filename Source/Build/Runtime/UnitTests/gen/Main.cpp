#include <any>
#include <array>
#include <ctime>
#include <fstream>
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
import Soup.Build;
import Soup.Build.Runtime;
import Monitor.Shared;
import json11;
import Soup.Test.Assert;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

#include "BuildHistoryCheckerTests.gen.h"
#include "BuildEvaluateEngineTests.gen.h"
#include "BuildGenerateEngineTests.gen.h"
#include "FileSystemStateTests.gen.h"

#include "Contracts/ValueTableManagerTests.gen.h"
#include "Contracts/ValueTableReaderTests.gen.h"
#include "Contracts/ValueTableWriterTests.gen.h"

#include "LocalUserConfig/LocalUserConfigExtensionsTests.gen.h"
#include "LocalUserConfig/LocalUserConfigTests.gen.h"

#include "OperationGraph/OperationGraphTests.gen.h"
#include "OperationGraph/OperationGraphManagerTests.gen.h"
#include "OperationGraph/OperationGraphReaderTests.gen.h"
#include "OperationGraph/OperationGraphWriterTests.gen.h"

#include "Recipe/PackageReferenceTests.gen.h"
#include "Recipe/RecipeExtensionsTests.gen.h"
#include "Recipe/RecipeTests.gen.h"
#include "Recipe/RecipeTomlTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildHistoryCheckerTests();
	state += RunBuildEvaluateEngineTests();
	state += RunBuildGenerateEngineTests();
	state += RunFileSystemStateTests();

	state += RunLocalUserConfigExtensionsTests();
	state += RunLocalUserConfigTests();

	state += RunValueTableManagerTests();
	state += RunValueTableReaderTests();
	state += RunValueTableWriterTests();

	state += RunOperationGraphTests();
	state += RunOperationGraphManagerTests();
	state += RunOperationGraphReaderTests();
	state += RunOperationGraphWriterTests();

	state += RunPackageReferenceTests();
	state += RunRecipeExtensionsTests();
	state += RunRecipeTests();
	state += RuneRecipeTomlTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
