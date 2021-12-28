#include <iostream>

import Samples.Cpp.StaticLibrary.Library;
using namespace Samples.Cpp.StaticLibrary.Library;

int main()
{
	std::cout << "Hello World, " << Helper::GetName() << " Style!" << std::endl;
	return 0;
}
