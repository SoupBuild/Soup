#include <any>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Opal.Extensions;
import SoupCore;
import json11;
import SoupTest;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;

#include "Api/SoupApiTests.gen.h"
#include "Api/SoupApiJsonModelsTests.gen.h"

#include "Build/Runner/BuildHistoryCheckerTests.gen.h"
#include "Build/Runner/BuildHistoryJsonTests.gen.h"
#include "Build/Runner/BuildHistoryTests.gen.h"
#include "Build/Runner/BuildHistoryManagerTests.gen.h"
#include "Build/Runner/BuildRunnerTests.gen.h"

#include "Config/LocalUserConfigExtensionsTests.gen.h"
#include "Config/LocalUserConfigJsonTests.gen.h"
#include "Config/LocalUserConfigTests.gen.h"

#include "Package/PackageManagerTests.gen.h"
#include "Package/PackageReferenceTests.gen.h"
#include "Package/RecipeBuilderTests.gen.h"
#include "Package/RecipeBuildManagerTests.gen.h"
#include "Package/RecipeExtensionsTests.gen.h"
#include "Package/RecipeJsonTests.gen.h"
#include "Package/RecipeTests.gen.h"
#include "Package/RecipeTomlTests.gen.h"

#include "Utils/PathTests.gen.h"
#include "Utils/SemanticVersionTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunSoupApiTests();
	state += RunSoupApiJsonModelsTests();

	state += RunBuildHistoryCheckerTests();
	state += RunBuildHistoryJsonTests();
	state += RunBuildHistoryTests();
	state += RunBuildHistoryManagerTests();
	state += RunBuildRunnerTests();

	state += RunLocalUserConfigExtensionsTests();
	state += RunLocalUserConfigJsonTests();
	state += RunLocalUserConfigTests();

	state += RunPackageManagerTests();
	state += RunPackageReferenceTests();
	state += RunRecipeBuilderTests();
	state += RunRecipeBuildManagerTests();
	state += RunRecipeExtensionsTests();
	state += RunRecipeJsonTests();
	state += RunRecipeTests();
	state += RunRecipeTomlTests();

	state += RunPathTests();
	state += RunSemanticVersionTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
