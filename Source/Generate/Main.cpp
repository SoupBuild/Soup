// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <stdio.h>
#include <stdexcept>
#include <iostream>

#include "GenerateEngine.h"

int main()
{
	try
	{
		GenerateEngine::Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
}