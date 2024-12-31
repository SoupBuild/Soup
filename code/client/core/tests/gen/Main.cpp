#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <map>
#include <optional>
#include <unordered_map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

import Monitor.Host;
import Opal;
import Soup.Core;
import Soup.Test.Assert;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

#include "utilities/TestHelpers.h"

#include "build/BuildEngineTests.gen.h"
#include "build/BuildEvaluateEngineTests.gen.h"
#include "build/BuildHistoryCheckerTests.gen.h"
#include "build/BuildLoadEngineTests.gen.h"
#include "build/BuildRunnerTests.gen.h"
#include "build/FileSystemStateTests.gen.h"
#include "build/PackageProviderTests.gen.h"
#include "build/RecipeBuildLocationManagerTests.gen.h"

#include "local-user-config/LocalUserConfigExtensionsTests.gen.h"
#include "local-user-config/LocalUserConfigTests.gen.h"

#include "operation-graph/OperationGraphTests.gen.h"
#include "operation-graph/OperationGraphManagerTests.gen.h"
#include "operation-graph/OperationGraphReaderTests.gen.h"
#include "operation-graph/OperationGraphWriterTests.gen.h"
#include "operation-graph/OperationResultsTests.gen.h"
#include "operation-graph/OperationResultsManagerTests.gen.h"
#include "operation-graph/OperationResultsReaderTests.gen.h"
#include "operation-graph/OperationResultsWriterTests.gen.h"

#include "package/PackageManagerTests.gen.h"

#include "recipe/PackageIdentifierTests.gen.h"
#include "recipe/PackageNameTests.gen.h"
#include "recipe/PackageReferenceTests.gen.h"
#include "recipe/RecipeExtensionsTests.gen.h"
#include "recipe/RecipeTests.gen.h"
#include "recipe/RecipeSMLTests.gen.h"

#include "value-table/ValueTableManagerTests.gen.h"
#include "value-table/ValueTableReaderTests.gen.h"
#include "value-table/ValueTableWriterTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildEngineTests();
	state += RunBuildEvaluateEngineTests();
	state += RunBuildHistoryCheckerTests();
	state += RunBuildLoadEngineTests();
	state += RunBuildRunnerTests();
	state += RunFileSystemStateTests();
	state += RunPackageProviderTests();
	state += RunRecipeBuildLocationManagerTests();

	state += RunLocalUserConfigExtensionsTests();
	state += RunLocalUserConfigTests();

	state += RunOperationGraphTests();
	state += RunOperationGraphManagerTests();
	state += RunOperationGraphReaderTests();
	state += RunOperationGraphWriterTests();
	state += RunOperationResultsTests();
	state += RunOperationResultsManagerTests();
	state += RunOperationResultsReaderTests();
	state += RunOperationResultsWriterTests();

	state += RunPackageManagerTests();

	state += RunPackageIdentifierTests();
	state += RunPackageNameTests();
	state += RunPackageReferenceTests();
	state += RunRecipeExtensionsTests();
	state += RunRecipeTests();
	state += RunRecipeSMLTests();

	state += RunValueTableManagerTests();
	state += RunValueTableReaderTests();
	state += RunValueTableWriterTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
