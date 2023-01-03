#pragma once

#include "ScriptEvaluate.h"

namespace Soup::Build::Generate
{
	class GenerateEngine
	{
	public:
		static void Run(const Path& soupTargetDirectory)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start: " + soupTargetDirectory.ToString());

			ScriptEvaluate::Run();

			// Load the parameters file
			// auto parametersFile = soupTargetDirectory + BuildConstants.GenerateParametersFileName;
			// if (!ValueTableManager.TryLoadState(parametersFile, out var parametersState))
			// {
			// 	Log.Error("Failed to load the parameter file: " + parametersFile.ToString());
			// 	throw std::runtime_error("Failed to load parameter file.");
			// }

			// // Load the read access file
			// auto readAccessFile = soupTargetDirectory + BuildConstants.GenerateReadAccessFileName;
			// auto readAccessList = new List<Path>();
			// if (!await PathListManager.TryLoadFileAsync(readAccessFile, readAccessList))
			// {
			// 	Log.Error("Failed to load the read access file: " + readAccessFile.ToString());
			// 	throw std::runtime_error("Failed to load read access file.");
			// }

			// // Load the write access file
			// auto writeAccessFile = soupTargetDirectory + BuildConstants.GenerateWriteAccessFileName;
			// auto writeAccessList = new List<Path>();
			// if (!PathListManager.TryLoadFileAsync(writeAccessFile, writeAccessList))
			// {
			// 	Log.Error("Failed to load the write access file: " + writeAccessFile.ToString());
			// 	throw std::runtime_error("Failed to load write access file.");
			// }

			// // Get the required input state from the parameters
			// auto languageExtensionContent = parametersState["LanguageExtensionPath"].AsString().ToString();
			// Path? languageExtensionPath = null;
			// if (!string.IsNullOrEmpty(languageExtensionContent))
			// {
			// 	languageExtensionPath = Path(languageExtensionContent);
			// }

			// auto packageDirectory = Path(parametersState["PackageDirectory"].AsString().ToString());

			// // Load the recipe file
			// auto recipeFile = packageDirectory + BuildConstants.RecipeFileName;
			// auto (isSuccess, recipe) = RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile);
			// if (!isSuccess)
			// {
			// 	Log::Error("Failed to load the recipe: " + recipeFile.ToString());
			// 	throw std::runtime_error("Failed to load recipe.");
			// }

			// // Combine all the dependencies shared state
			// auto dependenciesSharedState = LoadDependenciesSharedState(parametersState);

			// // Generate the set of build extension libraries
			// auto buildExtensionLibraries = GenerateBuildExtensionSet(languageExtensionPath, dependenciesSharedState);

			// // Start a new active state that is initialized to the recipe itself
			// auto activeState = new ValueTable();

			// // Initialize the Recipe Root Table
			// auto recipeState = recipe.Document.ToBuildValue();
			// activeState.Add("Recipe", new Value(recipeState));

			// // Initialize the Parameters Root Table
			// activeState.Add("Parameters", new Value(parametersState));

			// // Initialize the Dependencies Root Table
			// activeState.Add("Dependencies", new Value(dependenciesSharedState));

			// // Keep the extension libraries open while running the build system
			// // to ensure their memory is kept alive
			// OperationGraph evaluateGraph;
			// IValueTable sharedState;

			// Create a new build system for the requested build
			// auto buildTaskManager = BuildTaskManager();

			// // Run all build extension register callbacks
			// for (auto buildExtension : buildExtensionLibraries)
			// {
			// 	auto library = LoadPlugin(buildExtension);
			// 	FindAllCommands(library, buildTaskManager);
			// }

			// // Run the build
			// auto buildState = BuildState(
			// 	activeState,
			// 	_fileSystemState,
			// 	readAccessList,
			// 	writeAccessList);
			// buildTaskManager.Execute(buildState, soupTargetDirectory);

			// // Grab the build results so the dependency libraries can be released asap
			// evaluateGraph = buildState.BuildOperationGraph();
			// sharedState = buildState.SharedState;

			// Save the operation graph so the evaluate phase can load it
			// auto evaluateGraphFile = soupTargetDirectory + BuildConstants.EvaluateGraphFileName;
			// OperationGraphManager.SaveState(evaluateGraphFile, evaluateGraph, _fileSystemState);

			// Save the shared state that is to be passed to the downstream builds
			// auto sharedStateFile = soupTargetDirectory + BuildConstants.GenerateSharedStateFileName;
			// ValueTableManager.SaveState(sharedStateFile, sharedState);
			Log::Diag("Build generate end");
		}

	private:
		/// <summary>
		/// Using the parameters to resolve the dependency output folders, load up the shared state table and
		/// combine them into a single value table to be used as input the this generate phase.
		/// </summary>
		// ValueTable LoadDependenciesSharedState(IValueTable parametersTable)
		// {
		// 	var sharedDependenciesTable = new ValueTable();
		// 	if (parametersTable.TryGetValue("Dependencies", out var dependencyTableValue))
		// 	{
		// 		var dependenciesTable = dependencyTableValue.AsTable();
		// 		foreach (var dependencyTypeValue in dependenciesTable)
		// 		{
		// 			var dependencyType = dependencyTypeValue.Key;
		// 			var dependencies = dependencyTypeValue.Value.AsTable();
		// 			foreach (var dependencyValue in dependencies)
		// 			{
		// 				var dependencyName = dependencyValue.Key;
		// 				var dependency = dependencyValue.Value.AsTable();
		// 				var soupTargetDirectory = new Path(dependency["SoupTargetDirectory"].AsString());
		// 				var sharedStateFile = soupTargetDirectory + BuildConstants.GenerateSharedStateFileName;

		// 				// Load the shared state file
		// 				if (!ValueTableManager.TryLoadState(sharedStateFile, out var sharedStateTable))
		// 				{
		// 					Log.Error("Failed to load the shared state file: " + sharedStateFile.ToString());
		// 					throw new InvalidOperationException("Failed to load shared state file.");
		// 				}

		// 				// Add the shared build state from this child build into the correct
		// 				// table depending on the build type
		// 				var typedDependenciesTable = EnsureValueTable(sharedDependenciesTable, dependencyType);
		// 				typedDependenciesTable.Add(
		// 					dependencyName,
		// 					new Value(sharedStateTable));
		// 			}
		// 		}
		// 	}

		// 	return sharedDependenciesTable;
		// }
	};
}
