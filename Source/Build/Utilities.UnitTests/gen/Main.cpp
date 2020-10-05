#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

import Opal;
import Soup.Build.Utilities;
import Soup.Test.Assert;
import Soup.Test.Utilities;

using namespace Opal;
using namespace Opal::System;
using namespace Soup::Test;

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	if (state.FailCount > 0)
		return 1;
	else
		return 0;
}
