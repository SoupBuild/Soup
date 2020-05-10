import Antlr4Runtime;
import SoupCore;
import SoupTest;

using namespace SoupTest;

#include "Commands\BuildCommandTests.gen.h"
#include "Commands\InitializeCommandTests.gen.h"
#include "Commands\InstallCommandTests.gen.h"
#include "Commands\PackCommandTests.gen.h"
#include "Commands\PublishCommandTests.gen.h"
#include "Commands\VersionCommandTests.gen.h"
#include "Commands\ViewCommandTests.gen.h"

int main()
{
	std::cout << "Running Tests..." << std::endl;

	TestState state = { 0, 0 };

	state += RunBuildCommandTests();
	state += RunInitializeCommandTests();
	state += RunInstallCommandTests();
	state += RunPackCommandTests();
	state += RunPublishCommandTests();
	state += RunVersionCommandTests();
	state += RunViewCommandTests();

	std::cout << state.PassCount << " PASSED." << std::endl;
	std::cout << state.FailCount << " FAILED." << std::endl;

	return 0;
}
