#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Soup.Build.Extensions;
import SoupTest;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;

#include "RecipeLanguageVersionTests.gen.h"
#include "RecipeTypeTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunRecipeLanguageVersionTests();
	state += RunRecipeTypeTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
