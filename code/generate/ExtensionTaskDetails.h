// <copyright file="ExtensionTaskDetails.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::Generate
{
	class ExtensionTaskDetails
	{
	public:
		ExtensionTaskDetails(
			std::string name,
			Path scriptFile,
			std::optional<Path> bundlesFile,
			std::vector<std::string> runBeforeList,
			std::vector<std::string> runAfterList) :
			Name(std::move(name)),
			ScriptFile(std::move(scriptFile)),
			BundlesFile(std::move(bundlesFile)),
			RunBeforeList(std::move(runBeforeList)),
			RunAfterList(std::move(runAfterList)),
			RunAfterClosureList(),
			HasRun(false)
		{
		}

		std::string Name;
		Path ScriptFile;
		std::optional<Path> BundlesFile;
		std::vector<std::string> RunBeforeList;
		std::vector<std::string> RunAfterList;
		std::vector<std::string> RunAfterClosureList;
		bool HasRun;
	};
}
