#include <iostream>
#include <Library.h>

using namespace Samples::Cpp::DynamicLibrary::Library;

int main()
{
	std::cout << "Hello World, " << Helper::GetName() << " Style!" << std::endl;
	return 0;
}
