// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import std.core;
import Soup.Core;

#include "Program.h"

int main(int argc, char** argv)
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    return Soup::Client::Program::Run(std::move(args));
}
