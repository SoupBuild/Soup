// <copyright file="ExtensionManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ExtensionDetails.h"

namespace Soup::Core::Generate
{
	/// <summary>
	/// The extension manager
	/// </summary>
	class ExtensionManager
	{
	private:
		std::map<std::string, ExtensionDetails> _extensions;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ExtensionManager"/> class.
		/// </summary>
		 ExtensionManager() :
			_extensions()
		{
		}

		/// <summary>
		/// Register extension
		/// </summary>
		void RegisterExtension(
			std::string name,
			Path scriptFile,
			std::vector<std::string> runBeforeList,
			std::vector<std::string> runAfterList)
		{
			Log::Diag("RegisterExtension: " + name);

			auto extensionDetails = ExtensionDetails(
				name,
				std::move(scriptFile),
				std::move(runBeforeList),
				std::move(runAfterList));

			auto runBeforeMessage = std::stringstream();
			runBeforeMessage << "RunBefore [";
			bool isFirst = true;
			for (auto& value : extensionDetails.RunBeforeList)
			{
				if (!isFirst)
					runBeforeMessage << ", ";

				runBeforeMessage << "\"" << value << "\"";
				isFirst = false;
			}

			runBeforeMessage << "]";
			Log::Diag(runBeforeMessage.str());

			auto runAfterMessage = std::stringstream();
			runAfterMessage << "RunAfter [";
			isFirst = true;
			for (auto& value : extensionDetails.RunAfterList)
			{
				if (!isFirst)
					runAfterMessage << ", ";

				runAfterMessage << "\"" << value << "\"";
				isFirst = false;
			}

			runAfterMessage << "]";
			Log::Diag(runAfterMessage.str());

			auto insertResult = _extensions.emplace(std::move(name), std::move(extensionDetails));
			if (!insertResult.second)
			{
				Log::HighPriority("An extension with the provided name has already been registered: " + name);
				throw std::runtime_error("An extension with the provided name has already been registered");
			}
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		void Execute()
		{
			// Setup each extension to have a complete list of extensions that must run before itself
			// Note: this is required to combine other extensions run before lists with the extensions
			// own run after list
			for (auto& [key, extension] : _extensions)
			{
				// Copy their own run after list
				extension.RunAfterClosureList.insert(
					extension.RunAfterClosureList.end(),
					extension.RunAfterList.begin(),
					extension.RunAfterList.end());

				// Add ourself to all extensions in our run before list
				for (auto& runBefore : extension.RunBeforeList)
				{
					// Try to find the other extension
					auto beforeExtensionContainer = _extensions.find(runBefore);
					if (beforeExtensionContainer != _extensions.end())
					{
						beforeExtensionContainer->second.RunAfterClosureList.push_back(extension.Name);
					}
				}
			}

			auto runtimeOrderList = ValueList();
			auto extensionInfoTable = ValueTable();

			// Run all extensions in the order they were registered
			// ensuring they are run in the correct dependency order
			ExtensionDetails* currentExtension;
			while (TryFindNextExtension(currentExtension))
			{
				if (currentExtension == nullptr)
					throw std::runtime_error("TryFindNextExtension returned empty result");

				Log::Info("ExtensionStart: " + currentExtension->Name);

				// auto serviceCollection = new ServiceCollection();
				// serviceCollection.AddSingleton(typeof(IBuildState), state);
				// serviceCollection.AddSingleton(typeof(IValueFactory), new ValueFactory());
				// auto serviceProvider = serviceCollection.BuildServiceProvider();

				// auto extension = (SoupExtension)ActivatorUtilities.CreateInstance(serviceProvider, currentExtension.ExtensionType);

				// extension.Execute();

				Log::Info("ExtensionDone: " + currentExtension->Name);

				// Build the extension info
				// auto extensionInfo = ValueTable();
				// extensionInfo.Add("ActiveState", Value(state.ActiveStateImpl.Clone()));
				// extensionInfo.Add("SharedState", Value(state.SharedStateImpl.Clone()));

				// extensionInfoTable.Add(currentExtension.Name, Value(extensionInfo));
				// runtimeOrderList.Add(Value(currentExtension.Name));

				currentExtension->HasRun = true;
			}

			// auto generateInfoTable = ValueTable();
			// generateInfoTable.emplace("Version", Value("0.1"));
			// generateInfoTable.emplace("RuntimeOrder", Value(runtimeOrderList));
			// generateInfoTable.emplace("ExtensionInfo", Value(extensionInfoTable));

			// Save the runtime information
			// auto generateInfoStateFile = soupTargetDirectory + BuildConstants.GenerateExtensionInfoFileName;
			// Log::Info("Save Generate Info State: " + generateInfoStateFile.ToString());
			// ValueTableManager.SaveState(generateInfoStateFile, generateInfoTable);
		}

	private:
		/// <summary>
		/// Try to find the next extension that has yet to be run and is ready
		/// Returns false if all extensions have been run
		/// Throws error if we hit a deadlock
		/// </summary>
		bool TryFindNextExtension(ExtensionDetails*& extension)
		{
			// Find the next extension that is ready to be run
			bool hasAnyStillWaiting = false;
			for (auto& [key, extensionDetails] : _extensions)
			{
				// Check if this extension has run already,
				// if not check if all if all upstream extensions have finished
				if (!extensionDetails.HasRun)
				{
					hasAnyStillWaiting = true;

					// Check if all of their run after dependencies have already finished
					bool hasDependencyPending = false;
					for (auto& runBefore : extensionDetails.RunAfterClosureList)
					{
						auto findResult = _extensions.find(runBefore);
						if (findResult != _extensions.end() && !findResult->second.HasRun)
						{
							// Found a dependency that hasn't run, keep trying
							hasDependencyPending = true;
							break;
						}
					}

					// All dependencies have finished
					// Let's run this one
					if (!hasDependencyPending)
					{
						extension = &extensionDetails;
						return true;
					}
				}
			}

			if (hasAnyStillWaiting)
				throw std::runtime_error("Hit deadlock in build extension dependencies.");

			extension = nullptr;
			return false;
		}
	};
}
