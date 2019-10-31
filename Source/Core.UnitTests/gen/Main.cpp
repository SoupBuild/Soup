import std.core;
import Soup.Core;
import json11;

#include "../../Dependencies/SoupTest/Assert/SoupAssert.h"
#include "../../Dependencies/SoupTest/Assert/RunTest.h"
using namespace SoupTest;

#include "Build\BuildEngineTests.gen.h"
#include "Build\BuildStateCheckerTests.gen.h"
#include "Build\BuildStateJsonTests.gen.h"
#include "Build\BuildStateTests.gen.h"
#include "Build\BuildStateManagerTests.gen.h"

#include "Package\PackageReferenceTests.gen.h"
#include "Package\RecipeBuilderTests.gen.h"
#include "Package\RecipeBuildGeneratorTests.gen.h"
#include "Package\RecipeBuildManagerTests.gen.h"
#include "Package\RecipeExtensionsTests.gen.h"
#include "Package\RecipeLanguageVersionTests.gen.h"
#include "Package\RecipeJsonTests.gen.h"
#include "Package\RecipeTests.gen.h"
#include "Package\RecipeTypeTests.gen.h"

#include "Utils\PathTests.gen.h"
#include "Utils\SemanticVersionTests.gen.h"

#include "LocalUserConfigJsonTests.gen.h"
#include "LocalUserConfigTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildEngineTests();
	state += RunBuildStateCheckerTests();
	state += RunBuildStateJsonTests();
	state += RunBuildStateTests();
	state += RunBuildStateManagerTests();

	state += RunPackageReferenceTests();
	state += RunRecipeBuilderTests();
	state += RunRecipeBuildGeneratorTests();
	state += RunRecipeBuildManagerTests();
	state += RunRecipeExtensionsTests();
	state += RunRecipeLanguageVersionTests();
	state += RunRecipeJsonTests();
	state += RunRecipeTests();
	state += RunRecipeTypeTests();

	state += RunPathTests();
	state += RunSemanticVersionTests();
	
	state += RunLocalUserConfigJsonTests();
	state += RunLocalUserConfigTests();

	std::cout << state.FailCount << " FAILED." << std::endl;
	std::cout << state.PassCount << " PASSED." << std::endl;

	return 0;
}
