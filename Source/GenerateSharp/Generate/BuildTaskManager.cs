// <copyright file="BuildSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.Extensions.DependencyInjection;
using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Text;

namespace Soup.Build.Generate
{
	internal class BuildTaskContainer
	{
		public BuildTaskContainer(
			string name,
			Type taskType,
			IReadOnlyList<string> runBeforeList,
			IReadOnlyList<string> runAfterList)
		{
			Name = name;
			TaskType = taskType;
			RunBeforeList = runBeforeList;
			RunAfterList = runAfterList;
			RunAfterClosureList = new List<string>();
			HasRun = false;
		}

		public string Name { get; init; }
		public Type TaskType { get; init; }
		public IReadOnlyList<string> RunBeforeList { get; init; }
		public IReadOnlyList<string> RunAfterList { get; init; }
		public List<string> RunAfterClosureList { get; init; }
		public bool HasRun { get; set; }
	}

	/// <summary>
	/// The build system implementation
	/// </summary>
	internal class BuildTaskManager
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildTaskManager"/> class.
		/// </summary>
		public BuildTaskManager()
		{
			_tasks = new Dictionary<string, BuildTaskContainer>();
		}

		/// <summary>
		/// Register task
		/// </summary>
		public void RegisterTask(
			string name,
			Type taskType,
			IReadOnlyList<string> runBeforeList,
			IReadOnlyList<string> runAfterList)
		{
			Log.Diag("RegisterTask: " + name);

			var taskContainer = new BuildTaskContainer(
				name,
				taskType,
				runBeforeList,
				runAfterList);

			var runBeforeMessage = new StringBuilder();
			runBeforeMessage.Append("RunBefore [");
			bool isFirst = true;
			foreach (var value in taskContainer.RunBeforeList)
			{
				if (!isFirst)
					runBeforeMessage.Append(", ");

				runBeforeMessage.Append("\"");
				runBeforeMessage.Append(value);
				runBeforeMessage.Append("\"");
				isFirst = false;
			}

			runBeforeMessage.Append("]");
			Log.Diag(runBeforeMessage.ToString());

			var runAfterMessage = new StringBuilder();
			runAfterMessage.Append("RunAfter [");
			isFirst = true;
			foreach (var value in taskContainer.RunAfterList)
			{
				if (!isFirst)
					runAfterMessage.Append(", ");

				runAfterMessage.Append("\"");
				runAfterMessage.Append(value);
				runAfterMessage.Append("\"");
				isFirst = false;
			}

			runAfterMessage.Append("]");
			Log.Diag(runAfterMessage.ToString());

			var insertResult = _tasks.TryAdd(name, taskContainer);
			if (!insertResult)
			{
				Log.HighPriority("A task with the provided name has already been registered: " + name);
				throw new InvalidOperationException("A task with the provided name has already been registered");
			}
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		public void Execute(BuildState state, Path soupTargetDirectory)
		{
			// Setup each task to have a complete list of tasks that must run before itself
			// Note: this is required to combine other tasks run before lists with the tasks
			// own run after list
			foreach (var task in _tasks.Values)
			{
				// Copy their own run after list
				task.RunAfterClosureList.AddRange(task.RunAfterList);

				// Add ourself to all tasks in our run before list
				foreach (var runBefore in task.RunBeforeList)
				{
					// Try to find the other task
					if (_tasks.TryGetValue(runBefore, out var beforeTaskContainer))
					{
						beforeTaskContainer.RunAfterClosureList.Add(task.Name);
					}
				}
			}

			var runtimeOrderList = new ValueList();
			var taskInfoTable = new ValueTable();

			// Run all tasks in the order they were registered
			// ensuring they are run in the correct dependency order
			while (TryFindNextTask(out var currentTask))
			{
				if (ReferenceEquals(currentTask, null))
					throw new InvalidOperationException();

				Log.Info("TaskStart: " + currentTask.Name);

				var serviceCollection = new ServiceCollection();
				serviceCollection.AddSingleton(typeof(IBuildState), state);
				serviceCollection.AddSingleton(typeof(IValueFactory), new ValueFactory());
				var serviceProvider = serviceCollection.BuildServiceProvider();

				var task = (IBuildTask)ActivatorUtilities.CreateInstance(serviceProvider, currentTask.TaskType);

				task.Execute();

				Log.Info("TaskDone: " + currentTask.Name);

				// Write the output from the task to be used for debugging
				var activeStateFile = soupTargetDirectory + new Path($"ActiveState_{currentTask.Name}.bvt");
				var sharedStateFile = soupTargetDirectory + new Path($"SharedState_{currentTask.Name}.bvt");
				ValueTableManager.SaveState(activeStateFile, state.ActiveState);
				ValueTableManager.SaveState(sharedStateFile, state.SharedState);

				// Build the task info
				var taskInfo = new ValueTable();
				taskInfo.Add("ActiveState", new Value(state.ActiveStateImpl.Clone()));
				taskInfo.Add("SharedState", new Value(state.SharedStateImpl.Clone()));

				taskInfoTable.Add(currentTask.Name, new Value(taskInfo));
				runtimeOrderList.Add(new Value(currentTask.Name));

				currentTask.HasRun = true;
			}

			var generateInfoTable = new ValueTable();
			generateInfoTable.Add("Version", new Value("0.1"));
			generateInfoTable.Add("RuntimeOrder", new Value(runtimeOrderList));
			generateInfoTable.Add("TaskInfo", new Value(taskInfoTable));

			// Save the runtime information
			var generateInfoStateFile = soupTargetDirectory + new Path($"GenerateInfo.bvt");
			ValueTableManager.SaveState(generateInfoStateFile, generateInfoTable);
		}

		/// <summary>
		/// Try to find the next task that has yet to be run and is ready
		/// Returns false if all tasks have been run
		/// Throws error if we hit a deadlock
		/// </summary>
		private bool TryFindNextTask(out BuildTaskContainer? task)
		{
			// Find the next task that is ready to be run
			bool hasAnyStillWaiting = false;
			foreach (var activeTask in _tasks)
			{
				// Check if this task has run already, 
				// if not check if all if all upstream tasks have finished
				var taskContainer = activeTask.Value;
				if (!taskContainer.HasRun)
				{
					hasAnyStillWaiting = true;

					// Check if all of their run after dependencies have already finished
					bool hasDependencyPending = false;
					foreach (var runBefore in taskContainer.RunAfterClosureList)
					{
						if (_tasks.TryGetValue(runBefore, out var findResult) && !findResult.HasRun)
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
						task = taskContainer;
						return true;
					}
				}
			}

			if (hasAnyStillWaiting)
				throw new InvalidOperationException("Hit deadlock in build task dependencies.");

			task = null;
			return false;
		}

		private IDictionary<string, BuildTaskContainer> _tasks;
	};
}
