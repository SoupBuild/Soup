#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

import Opal;
import Soup.Cpp.Compiler;
import Soup.Cpp.Compiler.Clang;
import Soup.Test.Assert;
import Soup.Build;
import Soup.Build.Utilities;
import Soup.Build.Runtime;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

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

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
