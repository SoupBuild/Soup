#include <iostream>

int main()
{
#ifdef SPECIAL_BUILD
	std::cout << "Hello World, Soup Style!" << std::endl;
#else
	std::cout << "Hello World..." << std::endl;
#endif
	return 0;
}
