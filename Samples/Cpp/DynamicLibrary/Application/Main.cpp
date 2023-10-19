#include <iostream>
#include <Library.h>

using namespace Samples::Cpp::DynamicLibrary::Library;

int main()
{
	if (Helper::IsAwesome())
		std::cout << "Hello World, Soup Style!" << std::endl;
	else
		std::cout << "Hello World" << std::endl;

	return 0;
}
