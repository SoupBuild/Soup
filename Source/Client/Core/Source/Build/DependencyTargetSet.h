// <copyright file="DependencyTargetSet.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The dependency target set used to track all directories and macros in use by a package build runtime
	/// </summary>
	class DependencyTargetSet
	{
	public:
		std::set<Path> DirectRealDirectories;
		std::set<Path> RecursiveMacroDirectories;
		std::map<std::string, std::string> RecursiveMacros;

	public:
		DependencyTargetSet(
			std::set<Path> directRealDirectories,
			std::set<Path> recursiveMacroDirectories,
			std::map<std::string, std::string> recursiveMacros) :
			DirectRealDirectories(std::move(directRealDirectories)),
			RecursiveMacroDirectories(std::move(recursiveMacroDirectories)),
			RecursiveMacros(std::move(recursiveMacros))
		{
		}
	};
}
