// <copyright file="TaskGraphPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;

namespace SoupView.ViewModel
{
	internal class TaskGraphPageModel : Observable
	{
		private GraphNode selectedNode = null;
		private TaskDetailsViewModel selectedTask = null;
		private string errorBarMessage = string.Empty;
		private uint uniqueId = 0;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNode>> graph = null;
		private Dictionary<uint, TaskDetailsViewModel> taskDetailsLookup = new Dictionary<uint, TaskDetailsViewModel>();

		public string ErrorBarMessage
		{
			get { return errorBarMessage; }
			set
			{
				if (value != errorBarMessage)
				{
					errorBarMessage = value;
					NotifyPropertyChanged();
				}
			}
		}

		public IList<IList<GraphNode>> Graph
		{
			get { return graph; }
			set
			{
				if (value != graph)
				{
					graph = value;
					NotifyPropertyChanged();
				}
			}
		}

		public GraphNode SelectedNode
		{
			get { return selectedNode; }
			set
			{
				if (value != selectedNode)
				{
					selectedNode = value;
					NotifyPropertyChanged();
					SelectedTask = this.taskDetailsLookup[selectedNode.Id];
				}
			}
		}

		public TaskDetailsViewModel SelectedTask
		{
			get { return selectedTask; }
			set
			{
				if (value != selectedTask)
				{
					selectedTask = value;
					NotifyPropertyChanged();
				}
			}
		}

		public bool IsErrorBarOpen
		{
			get { return isErrorBarOpen; }
			set
			{
				if (value != isErrorBarOpen)
				{
					isErrorBarOpen = value;
					NotifyPropertyChanged();
				}
			}
		}

		public async Task LoadProjectAsync(Path recipeFilePath)
		{
			var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFilePath);
			if (loadResult.IsSuccess)
			{
				var packageDirectory = recipeFilePath.GetParent();
				var targetPath = await GetTargetPathAsync(packageDirectory);

				var soupTargetDirectory = targetPath + new Path(".soup/");

				var generateInfoStateFile = soupTargetDirectory + BuildConstants.GenerateTaskInfoFileName;
				if (!ValueTableManager.TryLoadState(generateInfoStateFile, out var generateInfoTable))
				{
					NotifyError($"Failed to load Value Table: {generateInfoStateFile}");
					return;
				}

				Graph = BuildGraph(generateInfoTable);
			}
			else
			{
				NotifyError($"Failed to load Recipe file: {recipeFilePath}");
			}
		}

		private void NotifyError(string message)
		{
			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private IList<IList<GraphNode>> BuildGraph(IValueTable generateInfoTable)
		{
			var activeGraph = new List<IList<GraphNode>>();
			this.taskDetailsLookup.Clear();
			this.uniqueId = 1;

			if (!generateInfoTable.TryGetValue("RuntimeOrder", out var runtimeOrderList) || runtimeOrderList.Type != Soup.Build.ValueType.List)
			{
				NotifyError($"Generate Info Table missing RuntimeOrder List");
				return activeGraph;
			}

			if (!generateInfoTable.TryGetValue("TaskInfo", out var taskInfoTable) || taskInfoTable.Type != Soup.Build.ValueType.Table)
			{
				NotifyError($"Generate Info Table missing TaskInfo Table");
				return activeGraph;
			}

			BuildGraph(runtimeOrderList.AsList(), taskInfoTable.AsTable(), activeGraph);

			return activeGraph;
		}

		private void BuildGraph(
			IValueList runtimeOrderList,
			IValueTable taskInfoTable,
			IList<IList<GraphNode>> activeGraph)
		{
			// Add each task to its own column
			foreach (var taskNameValue in runtimeOrderList)
			{
				var taskName = taskNameValue.AsString();

				var node = new GraphNode(taskName, this.uniqueId++);

				// Find the Task Info
				var taskInfo = taskInfoTable[taskName].AsTable();

				this.taskDetailsLookup.Add(node.Id, new TaskDetailsViewModel(taskName, taskInfo));

				// Add the new column at the start
				var column = new List<GraphNode>()
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
