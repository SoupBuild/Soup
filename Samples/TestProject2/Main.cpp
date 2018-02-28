#include "..\TestProject1\Source\Color.h"
#include <iostream>

int main()
{
	TestProject1::Color color;

	std::wcout << color.GetValue() << std::endl;

	return 0;
}