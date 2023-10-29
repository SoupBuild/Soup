// <copyright file="TaskGraphViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Threading;
using Opal;
using ReactiveUI;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using ValueType = Soup.Build.Utilities.ValueType;

namespace Soup.View.ViewModels
{
	public class TaskGraphViewModel : ViewModelBase
	{
		private GraphNodeViewModel? selectedNode = null;
		private TaskDetailsViewModel? selectedTask = null;
		private string errorBarMessage = string.Empty;
		private uint uniqueId = 0;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNodeViewModel>>? graph = null;
		private Dictionary<uint, TaskDetailsViewModel> taskDetailsLookup = new Dictionary<uint, TaskDetailsViewModel>();

		public string ErrorBarMessage
		{
			get => errorBarMessage;
			set => this.RaiseAndSetIfChanged(ref errorBarMessage, value);
		}

		public IList<IList<GraphNodeViewModel>>? Graph
		{
			get => graph;
			set => this.RaiseAndSetIfChanged(ref graph, value);
		}

		public GraphNodeViewModel? SelectedNode
		{
			get => selectedNode;
			set
			{
				if (this.CheckRaiseAndSetIfChanged(ref selectedNode, value))
				{
					if (selectedNode != null)
					{
						SelectedTask = this.taskDetailsLookup[selectedNode.Id];
					}
					else
					{
						SelectedTask = null;
					}
				}
			}
		}

		public TaskDetailsViewModel? SelectedTask
		{
			get => selectedTask;
			set => this.RaiseAndSetIfChanged(ref selectedTask, value);
		}

		public bool IsErrorBarOpen
		{
			get => isErrorBarOpen;
			set => this.RaiseAndSetIfChanged(ref isErrorBarOpen, value);
		}

		public async Task LoadProjectAsync(Path? packageFolder)
		{
			Graph = null;

			var activeGraph = await Task.Run(async () =>
			{
				if (packageFolder is not null)
				{
					var recipeFile = packageFolder + BuildConstants.RecipeFileName;
					var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile);
					if (loadResult.IsSuccess)
					{
						var targetPath = await GetTargetPathAsync(packageFolder);

						var soupTargetDirectory = targetPath + new Path(".soup/");

						var generateInfoStateFile = soupTargetDirectory + BuildConstants.GenerateInfoFileName;
						if (!ValueTableManager.TryLoadState(generateInfoStateFile, out var generateInfoTable))
						{
							NotifyError($"Failed to load Value Table: {generateInfoStateFile}");
							return null;
						}

						return BuildGraph(generateInfoTable);
					}
					else
					{
						NotifyError($"Failed to load Recipe file: {packageFolder}");
					}
				}

				return null;
			});

			Graph = activeGraph;
		}

		private void NotifyError(string message)
		{
			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private IList<IList<GraphNodeViewModel>> BuildGraph(ValueTable generateInfoTable)
		{
			var activeGraph = new List<IList<GraphNodeViewModel>>();
			this.taskDetailsLookup.Clear();
			this.uniqueId = 1;

			if (!generateInfoTable.TryGetValue("RuntimeOrder", out var runtimeOrderList) || runtimeOrderList.Type != ValueType.List)
			{
				NotifyError($"Generate Info Table missing RuntimeOrder List");
				return activeGraph;
			}

			if (!generateInfoTable.TryGetValue("TaskInfo", out var taskInfoTable) || taskInfoTable.Type != ValueType.Table)
			{
				NotifyError($"Generate Info Table missing TaskInfo Table");
				return activeGraph;
			}

			if (!generateInfoTable.TryGetValue("GlobalState", out var globalStateTable) || globalStateTable.Type != ValueType.Table)
			{
				NotifyError($"Generate Info Table missing GlobalState Table");
				return activeGraph;
			}

			BuildGraph(runtimeOrderList.AsList(), taskInfoTable.AsTable(), globalStateTable.AsTable(), activeGraph);

			return activeGraph;
		}

		private void BuildGraph(
			ValueList runtimeOrderList,
			ValueTable taskInfoTable,
			ValueTable globalStateTable,
			IList<IList<GraphNodeViewModel>> activeGraph)
		{
			// Add each task to its own column
			foreach (var taskNameValue in runtimeOrderList)
			{
				var taskName = taskNameValue.AsString();
				var taskToolTip = taskName;

				var node = new GraphNodeViewModel(taskName, taskToolTip, this.uniqueId++);

				// Find the Task Info
				var taskInfo = taskInfoTable[taskName].AsTable();

				// TODO: Have a custom view for the global state
				taskInfo["GlobalState"] = new Value(globalStateTable);

				this.taskDetailsLookup.Add(node.Id, new TaskDetailsViewModel(taskInfo));

				// Add the new column at the start
				var column = new List<GraphNodeViewModel>()
				{
					node
				};
				activeGraph.Add(column);
			}
		}

		private async Task<Path> GetTargetPathAsync(Path packageDirectory)
		{
			var processInfo = new ProcessStartInfo("C:\\Program Files\\SoupBuild\\Soup\\Soup\\Soup.exe", $"target {packageDirectory}")
			{
				RedirectStandardOutput = true,
				CreateNoWindow = true,
			};
			var process = new Process()
			{
				StartInfo = processInfo,
			};

			process.Start();

			await process.WaitForExitAsync();

			if (process.ExitCode != 0)
			{
				NotifyError($"Soup process exited with error: {process.ExitCode}");
				throw new InvalidOperationException();
			}

			var output = await process.StandardOutput.ReadToEndAsync();
			return new Path(output);
		}
	}
}
