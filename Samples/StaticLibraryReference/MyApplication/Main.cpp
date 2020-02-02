#include <iostream>

import MyLibrary;
using namespace MyLibrary;

int main()
{
	std::cout << "Hello World, " << Helper::GetName() << " Style!" << std::endl;
	return 0;
}
