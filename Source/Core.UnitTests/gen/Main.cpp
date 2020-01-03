#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import SoupCore;
import json11;
import SoupTest;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;
import SoupTestUtilities;

#include "Build/Runner/BuildHistoryCheckerTests.gen.h"
#include "Build/Runner/BuildHistoryJsonTests.gen.h"
#include "Build/Runner/BuildHistoryTests.gen.h"
#include "Build/Runner/BuildHistoryManagerTests.gen.h"
#include "Build/Runner/BuildRunnerTests.gen.h"

#include "Build/Tasks/BuildTaskTests.gen.h"
#include "Build/Tasks/RecipeBuildTaskTests.gen.h"
#include "Build/Tasks/ResolveToolsTaskTests.gen.h"
#include "Build/Tasks/StandardLibraryIncludeTaskTests.gen.h"

#include "Config/LocalUserConfigExtensionsTests.gen.h"
#include "Config/LocalUserConfigJsonTests.gen.h"
#include "Config/LocalUserConfigTests.gen.h"

#include "Package/PackageReferenceTests.gen.h"
#include "Package/RecipeBuilderTests.gen.h"
#include "Package/RecipeBuildManagerTests.gen.h"
#include "Package/RecipeExtensionsTests.gen.h"
#include "Package/RecipeLanguageVersionTests.gen.h"
#include "Package/RecipeJsonTests.gen.h"
#include "Package/RecipeTests.gen.h"
#include "Package/RecipeTypeTests.gen.h"

#include "Utils/PathTests.gen.h"
#include "Utils/SemanticVersionTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildHistoryCheckerTests();
	state += RunBuildHistoryJsonTests();
	state += RunBuildHistoryTests();
	state += RunBuildHistoryManagerTests();
	state += RunBuildRunnerTests();

	state += RunBuildTaskTests();
	state += RunRecipeBuildTaskTests();
	state += RunResolveToolsTaskTests();
	state += RunStandardLibraryIncludeTaskTests();

	state += RunLocalUserConfigExtensionsTests();
	state += RunLocalUserConfigJsonTests();
	state += RunLocalUserConfigTests();

	state += RunPackageReferenceTests();
	state += RunRecipeBuilderTests();
	state += RunRecipeBuildManagerTests();
	state += RunRecipeExtensionsTests();
	state += RunRecipeLanguageVersionTests();
	state += RunRecipeJsonTests();
	state += RunRecipeTests();
	state += RunRecipeTypeTests();

	state += RunPathTests();
	state += RunSemanticVersionTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
