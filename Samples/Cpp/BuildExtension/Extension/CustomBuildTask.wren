// <copyright file="CustomBuildTask.wren" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import "soup" for Soup, SoupTask
import "Soup.Build.Utils:./ListExtensions" for ListExtensions
import "Soup.Build.Utils:./MapExtensions" for MapExtensions

class CustomBuildTask is SoupTask {
	/// <summary>
	/// Get the run before list
	/// </summary>
	static runBefore { [
		"BuildTask"
	] }

	/// <summary>
	/// Get the run after list
	/// </summary>
	static runAfter { [] }

	static evaluate() {
		Soup.info("Running Before Build!")

		// Get the build table
		var buildTable = MapExtensions.EnsureTable(Soup.activeState, "Build")

		// As a sample of what a build extension can do we set a new
		// preprocessor definition to influence the build
		var preprocessorDefinitions = [
			"SPECIAL_BUILD",
		]
		
		ListExtensions.Append(
			MapExtensions.EnsureList(buildTable, "PreprocessorDefinitions"),
			preprocessorDefinitions)
	}
}
