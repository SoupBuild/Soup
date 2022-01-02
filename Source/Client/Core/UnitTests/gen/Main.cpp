#include <any>
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
import Soup.Core;
import json11;
import Monitor.Host;
import Soup.Test.Assert;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

#include "BuildHistoryCheckerTests.gen.h"
#include "BuildEvaluateEngineTests.gen.h"
#include "FileSystemStateTests.gen.h"

#include "ValueTable/ValueTableManagerTests.gen.h"
#include "ValueTable/ValueTableReaderTests.gen.h"
#include "ValueTable/ValueTableWriterTests.gen.h"

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

#include "Package/PackageManagerTests.gen.h"
#include "Package/RecipeBuilderTests.gen.h"
#include "Package/RecipeBuildManagerTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunPackageManagerTests();
	state += RunRecipeBuilderTests();
	state += RunRecipeBuildManagerTests();

	state += RunBuildHistoryCheckerTests();
	state += RunBuildEvaluateEngineTests();
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
	state += RunRecipeTomlTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
