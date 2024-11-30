// <copyright file="CustomBuildTask.wren" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

import "soup" for Soup, SoupTask
import "Soup|Build.Utils:./Path" for Path
import "Soup|Build.Utils:./ListExtensions" for ListExtensions
import "Soup|Build.Utils:./MapExtensions" for MapExtensions
import "Soup|Build.Utils:./SharedOperations" for SharedOperations

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

	/// <summary>
	/// Core Evaluate
	/// </summary>
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

		var contextTable = Soup.globalState["Context"]
		var packageRoot = Path.new(contextTable["PackageDirectory"])

		CustomBuildTask.CreateCustomToolOperation(packageRoot)
	}

	/// <summary>
	/// Create a build operation that will create a directory
	/// </summary>
	static CreateCustomToolOperation(workingDirectory) {
		// Discover the dependency tool
		var toolExecutable = SharedOperations.ResolveRuntimeDependencyRunExectable("Samples.SimpleBuildExtension.Tool")

		var title = "Run Custom Tool"

		var program = Path.new(toolExecutable)
		var inputFiles = []
		var outputFiles = []

		// Build the arguments
		var arguments = []

		Soup.createOperation(
			title,
			program.toString,
			arguments,
			workingDirectory.toString,
			ListExtensions.ConvertFromPathList(inputFiles),
			ListExtensions.ConvertFromPathList(outputFiles))
	}
}
