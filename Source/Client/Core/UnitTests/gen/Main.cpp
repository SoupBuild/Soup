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

#include "Utils/TestHelpers.h"

#include "Build/BuildEngineTests.gen.h"
#include "Build/BuildEvaluateEngineTests.gen.h"
#include "Build/BuildHistoryCheckerTests.gen.h"
#include "Build/BuildLoadEngineTests.gen.h"
#include "Build/BuildRunnerTests.gen.h"
#include "Build/FileSystemStateTests.gen.h"
#include "Build/PackageProviderTests.gen.h"
#include "Build/RecipeBuildLocationManagerTests.gen.h"

#include "LocalUserConfig/LocalUserConfigExtensionsTests.gen.h"
#include "LocalUserConfig/LocalUserConfigTests.gen.h"

#include "OperationGraph/OperationGraphTests.gen.h"
#include "OperationGraph/OperationGraphManagerTests.gen.h"
#include "OperationGraph/OperationGraphReaderTests.gen.h"
#include "OperationGraph/OperationGraphWriterTests.gen.h"
#include "OperationGraph/OperationResultsTests.gen.h"
#include "OperationGraph/OperationResultsManagerTests.gen.h"
#include "OperationGraph/OperationResultsReaderTests.gen.h"
#include "OperationGraph/OperationResultsWriterTests.gen.h"

#include "Package/PackageManagerTests.gen.h"

#include "Recipe/PackageIdentifierTests.gen.h"
#include "Recipe/PackageNameTests.gen.h"
#include "Recipe/PackageReferenceTests.gen.h"
#include "Recipe/RecipeExtensionsTests.gen.h"
#include "Recipe/RecipeTests.gen.h"
#include "Recipe/RecipeSMLTests.gen.h"

#include "ValueTable/ValueTableManagerTests.gen.h"
#include "ValueTable/ValueTableReaderTests.gen.h"
#include "ValueTable/ValueTableWriterTests.gen.h"

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
