// <copyright file="RecipeBuildCacheState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	class RecipeBuildCacheState
	{
	public:
		RecipeBuildCacheState(
			std::string name,
			Path targetDirectory,
			Path soupTargetDirectory,
			std::set<Path> recursiveChildTargetDirectorySet) :
			Name(std::move(name)),
			TargetDirectory(std::move(targetDirectory)),
			SoupTargetDirectory(std::move(soupTargetDirectory)),
			RecursiveChildTargetDirectorySet(std::move(recursiveChildTargetDirectorySet))
		{
		}

		std::string Name;
		Path TargetDirectory;
		Path SoupTargetDirectory;
		std::set<Path> RecursiveChildTargetDirectorySet;
	};
}
