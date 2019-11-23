// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import StandardLibrary;;
import SoupCore;
import SoupCompilerClang;
import SoupCompilerMSVC;

#include "Program.h"

int main(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	auto program = Soup::Client::Program();
	return program.Run(std::move(args));
}
