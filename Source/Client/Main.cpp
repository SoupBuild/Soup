// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import std.core;
import Soup.Core;
import Soup.Compiler.Clang;
import Soup.Compiler.MSVC;

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
