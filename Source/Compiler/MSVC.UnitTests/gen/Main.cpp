#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

import Opal;
import SoupCore;
import SoupCompilerMSVC;
import SoupTest;
import SoupTestUtilities;

using namespace Opal;
using namespace Opal::System;
using namespace SoupTest;

#include "CompilerArgumentBuilderTests.gen.h"
#include "CompilerTests.gen.h"
#include "LinkerArgumentBuilderTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunCompilerArgumentBuilderTests();
	state += RunCompilerTests();
	state += RunLinkerArgumentBuilderTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
