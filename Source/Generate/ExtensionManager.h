// <copyright file="ExtensionManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ExtensionTaskDetails.h"
#include "WrenHost.h"

namespace Soup::Core::Generate
{
	/// <summary>
	/// The extension manager
	/// </summary>
	class ExtensionManager
	{
	private:
		std::map<std::string, ExtensionTaskDetails> _tasks;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ExtensionManager"/> class.
		/// </summary>
		 ExtensionManager() :
			_tasks()
		{
		}

		/// <summary>
		/// Register extension task
		/// </summary>
		void RegisterExtensionTask(ExtensionTaskDetails extensionTaskDetails)
		{
			auto name = extensionTaskDetails.Name;
			Log::Diag("RegisterExtensionTask: " + name);

			auto runBeforeMessage = std::stringstream();
			runBeforeMessage << "RunBefore [";
			bool isFirst = true;
			for (auto& value : extensionTaskDetails.RunBeforeList)
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
			for (auto& value : extensionTaskDetails.RunAfterList)
			{
				if (!isFirst)
					runAfterMessage << ", ";

				runAfterMessage << "\"" << value << "\"";
				isFirst = false;
			}

			runAfterMessage << "]";
			Log::Diag(runAfterMessage.str());

			auto insertResult = _tasks.emplace(name, std::move(extensionTaskDetails));
			if (!insertResult.second)
			{
				Log::HighPriority("An extension task with the provided name has already been registered: " + name);
				throw std::runtime_error("An extension task with the provided name has already been registered");
			}
		}

		/// <summary>
		/// Execute all build extensions
		/// </summary>
		void Execute(GenerateState& state)
		{
			// Setup each extension to have a complete list of extensions that must run before itself
			// Note: this is required to combine other extensions run before lists with the extensions
			// own run after list
			for (auto& [key, task] : _tasks)
			{
				// Copy their own run after list
				task.RunAfterClosureList.insert(
					task.RunAfterClosureList.end(),
					task.RunAfterList.begin(),
					task.RunAfterList.end());

				// Add ourself to all tasks in our run before list
				for (auto& runBefore : task.RunBeforeList)
				{
					// Try to find the other task
					auto beforeTaskContainer = _tasks.find(runBefore);
					if (beforeTaskContainer != _tasks.end())
					{
						beforeTaskContainer->second.RunAfterClosureList.push_back(task.Name);
					}
				}
			}

			auto runtimeOrderList = ValueList();
			auto extensionTaskInfoTable = ValueTable();

			// Run all tasks in the order they were registered
			// ensuring they are run in the correct dependency order
			ExtensionTaskDetails* currentTask;
			while (TryFindNextTask(currentTask))
			{
				if (currentTask == nullptr)
					throw std::runtime_error("TryFindNextTask returned empty result");

				// Create a Wren Host to evaluate the extension task
				auto host = std::make_unique<WrenHost>(currentTask->ScriptFile);
				host->InterpretMain();

				// Set the current state AFTER we initialize to prevent pre-loading
				host->SetState(state);

				Log::Info("TaskStart: " + currentTask->Name);

				host->EvaluateTask(currentTask->Name);

				Log::Info("TaskDone: " + currentTask->Name);

				// Get the final state to be passed to the next extension
				auto updatedActiveState = host->GetUpdatedActiveState();
				auto updatedSharedState = host->GetUpdatedSharedState();

				// Build the extension task info
				auto extensionTaskInfo = ValueTable();
				extensionTaskInfo.emplace("ActiveState", Value(updatedActiveState));
				extensionTaskInfo.emplace("SharedState", Value(updatedSharedState));

				extensionTaskInfoTable.emplace(currentTask->Name, Value(std::move(extensionTaskInfo)));
				runtimeOrderList.push_back(Value(currentTask->Name));

				// Mark the extension task completed
				currentTask->HasRun = true;

				// Update state for next extension task
				Log::Info("UpdateState");
				state.Update(std::move(updatedActiveState), std::move(updatedSharedState));
			}

			// Store the runtime information for easy debugging
			auto generateInfoTable = ValueTable();
			generateInfoTable.emplace("Version", Value("0.1"));
			generateInfoTable.emplace("RuntimeOrder", Value(std::move(runtimeOrderList)));
			generateInfoTable.emplace("TaskInfo", Value(std::move(extensionTaskInfoTable)));

			state.SetGenerateInfo(std::move(generateInfoTable));
		}

	private:
		/// <summary>
		/// Try to find the next task that has yet to be run and is ready
		/// Returns false if all tasks have been run
		/// Throws error if we hit a deadlock
		/// </summary>
		bool TryFindNextTask(ExtensionTaskDetails*& extensionTask)
		{
			// Find the next task that is ready to be run
			bool hasAnyStillWaiting = false;
			for (auto& [key, task] : _tasks)
			{
				// Check if this extension has run already,
				// if not check if all if all upstream extensions have finished
				if (!task.HasRun)
				{
					hasAnyStillWaiting = true;

					// Check if all of their run after dependencies have already finished
					bool hasDependencyPending = false;
					for (auto& runBefore : task.RunAfterClosureList)
					{
						auto findResult = _tasks.find(runBefore);
						if (findResult != _tasks.end() && !findResult->second.HasRun)
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
						extensionTask = &task;
						return true;
					}
				}
			}

			if (hasAnyStillWaiting)
				throw std::runtime_error("Hit deadlock in build extension dependencies.");

			extensionTask = nullptr;
			return false;
		}
	};
}
