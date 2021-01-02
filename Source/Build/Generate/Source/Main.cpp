// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <memory>
#include <string>
#include <stdexcept>
#include <vector>

import Opal;
import Soup.Build.Runtime;

using namespace Opal;

#include "Program.h"

int main(int argc, char** argv)
{
	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}

	auto program = Soup::Build::Generate::Program();
	return program.Run(std::move(args));
}
