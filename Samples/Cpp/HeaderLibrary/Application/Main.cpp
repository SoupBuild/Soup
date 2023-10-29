#include <iostream>
#include <Library.h>

using namespace Samples::Cpp::HeaderLibrary::Library;

int main()
{
	std::cout << "Hello World, " << Helper::GetName() << " Style!" << std::endl;
	return 0;
}
