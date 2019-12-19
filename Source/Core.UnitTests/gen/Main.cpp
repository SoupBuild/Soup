#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import SoupCore;
import json11;
import SoupTest;

using namespace SoupTest;

#include "Build\BuildEngineTests.gen.h"
#include "Build\BuildStateCheckerTests.gen.h"
#include "Build\BuildStateJsonTests.gen.h"
#include "Build\BuildStateTests.gen.h"
#include "Build\BuildStateManagerTests.gen.h"

#include "Config\LocalUserConfigExtensionsTests.gen.h"
#include "Config\LocalUserConfigJsonTests.gen.h"
#include "Config\LocalUserConfigTests.gen.h"

#include "Package\PackageReferenceTests.gen.h"
#include "Package\RecipeBuilderTests.gen.h"
#include "Package\RecipeBuildManagerTests.gen.h"
#include "Package\RecipeExtensionsTests.gen.h"
#include "Package\RecipeLanguageVersionTests.gen.h"
#include "Package\RecipeJsonTests.gen.h"
#include "Package\RecipeTests.gen.h"
#include "Package\RecipeTypeTests.gen.h"

#include "Utils\PathTests.gen.h"
#include "Utils\SemanticVersionTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildEngineTests();
	state += RunBuildStateCheckerTests();
	state += RunBuildStateJsonTests();
	state += RunBuildStateTests();
	state += RunBuildStateManagerTests();

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

	std::cout << state.FailCount << " FAILED." << std::endl;
	std::cout << state.PassCount << " PASSED." << std::endl;

	return 0;
}
