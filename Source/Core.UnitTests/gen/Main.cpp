import std.core;
import Antlr4.Runtime;
import Soup.Core;
import json11;

#include "../../../SoupTest/Assert/SoupAssert.h"
#include "../../../SoupTest/Assert/RunTest.h"
using namespace SoupTest;

#include "Package\PackageReferenceTests.gen.h"
#include "Package\RecipeManagerTests.gen.h"
#include "Package\RecipeTests.gen.h"
#include "Package\RecipeTypeTests.gen.h"

#include "Utils\PathTests.gen.h"
#include "Utils\SemanticVersionTests.gen.h"

int main()
{
    std::cout << "Running Tests..." << std::endl;

    TestState state = { 0, 0 };

    state += RunPackageReferenceTests();
    state += RunRecipeManagerTests();
    state += RunRecipeTests();
    state += RunRecipeTypeTests();

    state += RunPathTests();
    state += RunSemanticVersionTests();

    std::cout << state.FailCount << " FAILED." << std::endl;
    std::cout << state.PassCount << " PASSED." << std::endl;

    return 0;
}
