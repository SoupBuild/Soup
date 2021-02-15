﻿// <copyright file="BuildSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Generate
{
	internal class BuildTaskContainer
	{
		public BuildTaskContainer(
			IBuildTask task,
			IList<string> runBeforeList,
			IList<string> runAfterList)
		{
			Task = task;
			RunBeforeList = runBeforeList;
			RunAfterList = runAfterList;
			HasRun = false;
		}

		public IBuildTask Task { get; init; }
		public IList<string> RunBeforeList { get; init; }
		public IList<string> RunAfterList { get; init; }
		public IList<string> RunAfterClosureList { get; init; }
		public bool HasRun { get; init; }
	};

	/// <summary>
	/// The build system implementation
	/// </summary>
	internal class BuildSystem
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildSystem"/> class.
		/// </summary>
		public BuildSystem()
		{
			_tasks = new Dictionary<string, BuildTaskContainer>();
		}

		/// <summary>
		/// Register task
		/// </summary>
		public void RegisterTask(IBuildTask task)
		{
			var taskName = task->GetName();
			Log.Diag("RegisterTask: " + taskName);

			// TODO: Remove const casts
			var runBeforeList = Utilities::ReadOnlyStringListWrapper(
				ourTask->GetRunBeforeList());
			var runAfterList = Utilities::ReadOnlyStringListWrapper(
				ourTask->GetRunAfterList());
			var taskContainer = BuildTaskContainer(
				ourTask,
				runBeforeList.CopyAsStringVector(),
				runAfterList.CopyAsStringVector());

			stringstream runBeforeMessage;
			runBeforeMessage << "RunBefore [";
			bool isFirst = true;
			for (auto& name : taskContainer.RunBeforeList)
			{
				if (!isFirst)
					runBeforeMessage << ", ";

				runBeforeMessage << "\"" << name << "\"";
				isFirst = false;
			}

			runBeforeMessage << "]";
			Log.Diag(runBeforeMessage.str());

			stringstream runAfterMessage;
			runAfterMessage << "RunAfter [";
			isFirst = true;
			for (auto& name : taskContainer.RunAfterList)
			{
				if (!isFirst)
					runAfterMessage << ", ";

				runAfterMessage << "\"" << name << "\"";
				isFirst = false;
			}

			runAfterMessage << "]";
			Log.Diag(runAfterMessage.str());

			auto insertResult = _tasks.try_emplace(taskName, std::move(taskContainer));
			if (!insertResult.second)
			{
				Log::HighPriority("A task with the provided name has already been registered: " + taskName);
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		public void Execute(BuildState state)
		{
			// Setup each task to have a complete list of tasks that must run before itself
			// Note: this is required to combine other tasks run before lists with the tasks
			// own run after list
			for (auto& task : _tasks)
			{
				auto& taskContainer = task.second;

				// Copy their own run after list
				std::copy(
					taskContainer.RunAfterList.begin(),
					taskContainer.RunAfterList.end(),
					std::back_inserter(taskContainer.RunAfterClosureList));

				// Add ourself to all tasks in our run before list
				for (auto& runBefore : taskContainer.RunBeforeList)
				{
					// Try to find the other task
					auto findResult = _tasks.find(runBefore);
					if (findResult != _tasks.end())
					{
						auto& beforeTaskContainer = findResult->second;
						beforeTaskContainer.RunAfterClosureList.push_back(task.first);
					}
				}
			}

			// Run all tasks in the order they were registered
			// ensuring they are run in the correct dependency order
			auto currentTask = FindNextTask();
			while (currentTask != _tasks.end())
			{
				Log.Info("TaskStart: " + currentTask->first);
				auto status = currentTask->second.Task->TryExecute(state);
				if (status != ApiCallResult::Success)
				{
					Log.Error("TaskFailed: " + std::to_string(static_cast<int64_t>(status)));
					throw std::runtime_error("Task Failed");
				}
				else
				{
					Log.Info("TaskDone: " + currentTask->first);
				}

				state.LogActive();
				currentTask->second.HasRun = true;
				currentTask = FindNextTask(); 
			}
		}

		/// <summary>
		/// Try to find the next task that has yet to be run and is ready
		/// Returns false if all tasks have been run
		/// Throws error if we hit a deadlock
		/// </summary>
		private std::unordered_map<string, BuildTaskContainer>::iterator FindNextTask()
		{
			// Find the next task that is ready to be run
			bool hasAnyStillWaiting = false;
			auto taskItr = _tasks.begin();
			for (;taskItr != _tasks.end(); taskItr++)
			{
				// Check if this task has run already, 
				// if not check if all if all upstream tasks have finished
				auto& taskContainer = taskItr->second;
				if (!taskContainer.HasRun)
				{
					hasAnyStillWaiting = true;

					// Check if all of their run after dependencies have already finished
					bool hasDependencyPending = false;
					for (auto& runBefore : taskContainer.RunAfterClosureList)
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
						return taskItr;
				}
			}

			if (!hasAnyStillWaiting)
				return _tasks.end();
			else
				throw std::runtime_error("Hit deadlock in build task dependencies.");
		}

		private IDictionary<string, BuildTaskContainer> _tasks;
	};
}
