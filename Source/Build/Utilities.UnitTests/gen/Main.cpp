#include <any>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Soup.Build.Utilities;
import Soup.Test.Assert;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	// Touch stamp file to ensure incremental builds work
	auto testFile = std::fstream("TestHarness.stamp", std::fstream::out);
	testFile << "TOUCH";

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
