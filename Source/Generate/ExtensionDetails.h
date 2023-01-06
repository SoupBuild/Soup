// <copyright file="ExtensionDetails.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::Generate
{
	class ExtensionDetails
	{
	public:
		ExtensionDetails(
			std::string name,
			std::vector<std::string> runBeforeList,
			std::vector<std::string> runAfterList) :
			Name(std::move(name)),
			RunBeforeList(std::move(runBeforeList)),
			RunAfterList(std::move(runAfterList)),
			RunAfterClosureList(),
			HasRun(false)
		{
		}

		std::string Name;
		std::vector<std::string> RunBeforeList;
		std::vector<std::string> RunAfterList;
		std::vector<std::string> RunAfterClosureList;
		bool HasRun;
	};
}
