import std.core;
import Antlr4.Runtime;
import Soup.Core;

#include "SoupAssert.h"
#include "RunTest.h"

#include "Package\PackageReferenceTests.gen.h"
#include "Package\RecipeTests.gen.h"
#include "Package\RecipeTypeTests.gen.h"

#include "Utils\SemanticVersionTests.gen.h"

int main()
{
    std::cout << "Running Tests..." << std::endl;

    TestState state = { 0, 0 };

    state += RunPackageReferenceTests();
    state += RunRecipeTests();
    state += RunRecipeTypeTests();

    state += RunSemanticVersionTests();

    std::cout << state.FailCount << " FAILED." << std::endl;
    std::cout << state.PassCount << " PASSED." << std::endl;

    return 0;
}
