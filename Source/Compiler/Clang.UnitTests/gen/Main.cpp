import std.core;
import Antlr4.Runtime;
import Soup.Core;
import Soup.Compiler.Clang;

#include "../../../../SoupTest/Assert/SoupAssert.h"
#include "../../../../SoupTest/Assert/RunTest.h"

#include "ArgumentBuilderTests.gen.h"

int main()
{
    std::cout << "Running Tests..." << std::endl;

    TestState state = { 0, 0 };

    state += RunArgumentBuilderTests();

    std::cout << state.FailCount << " FAILED." << std::endl;
    std::cout << state.PassCount << " PASSED." << std::endl;

    return 0;
}
