// <copyright file="TaskGraphViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using GraphShape;
using ReactiveUI;
using Soup.Build.Utilities;
using Soup.View.Views;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Path = Opal.Path;
using ValueType = Soup.Build.Utilities.ValueType;

namespace Soup.View.ViewModels;

public class TaskGraphViewModel : ContentPaneViewModel
{
	private sealed class TaskDetails
	{
		public required string Name { get; set; }
		public required uint Id { get; set; }
		public required ValueTable TaskInfo { get; set; }
		public required List<TaskDetails> Children { get; init; }
	}

	private GraphNodeViewModel? selectedNode;
	private TaskDetailsViewModel? selectedTask;
	private uint uniqueId;
	private IList<GraphNodeViewModel>? graph;
	private readonly Dictionary<uint, TaskDetailsViewModel> taskDetailsLookup = [];

	public IList<GraphNodeViewModel>? Graph
	{
		get => this.graph;
		private set => this.RaiseAndSetIfChanged(ref this.graph, value);
	}

	public GraphNodeViewModel? SelectedNode
	{
		get => this.selectedNode;
		set
		{
			if (CheckRaiseAndSetIfChanged(ref this.selectedNode, value))
			{
				this.SelectedTask = this.selectedNode is not null ? this.taskDetailsLookup[this.selectedNode.Id] : null;
			}
		}
	}

	public TaskDetailsViewModel? SelectedTask
	{
		get => this.selectedTask;
		set => this.RaiseAndSetIfChanged(ref this.selectedTask, value);
	}

	public async Task LoadProjectAsync(Path? packageFolder, string? owner)
	{
		this.Graph = null;

		var activeGraph = await Task.Run(async () =>
		{
			if (packageFolder is not null)
			{
				var recipeFile = packageFolder + BuildConstants.RecipeFileName;
				var (isSuccess, result) = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile);
				if (isSuccess)
				{
					var targetPath = await GetTargetPathAsync(packageFolder, owner);

					var soupTargetDirectory = targetPath + new Path("./.soup/");

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

		this.Graph = activeGraph;
	}

	private List<GraphNodeViewModel>? BuildGraph(ValueTable generateInfoTable)
	{
		this.taskDetailsLookup.Clear();
		this.uniqueId = 1;

		if (!generateInfoTable.TryGetValue("RuntimeOrder", out var runtimeOrderList) || runtimeOrderList.Type != ValueType.List)
		{
			NotifyError($"Generate Info Table missing RuntimeOrder List");
			return null;
		}

		if (!generateInfoTable.TryGetValue("TaskInfo", out var taskInfoTable) || taskInfoTable.Type != ValueType.Table)
		{
			NotifyError($"Generate Info Table missing TaskInfo Table");
			return null;
		}

		if (!generateInfoTable.TryGetValue("GlobalState", out var globalStateTable) || globalStateTable.Type != ValueType.Table)
		{
			NotifyError($"Generate Info Table missing GlobalState Table");
			return null;
		}

		var activeGraph = BuildGraph(runtimeOrderList.AsList(), taskInfoTable.AsTable(), globalStateTable.AsTable());

		return activeGraph;
	}

	private List<GraphNodeViewModel> BuildGraph(
		ValueList runtimeOrderList,
		ValueTable taskInfoTable,
		ValueTable globalStateTable)
	{
		var tasks = new Dictionary<string, TaskDetails>();

		// Add each task to its own column
		foreach (var taskNameValue in runtimeOrderList)
		{
			var taskName = taskNameValue.AsString();

			// Find the Task Info
			var taskInfo = taskInfoTable[taskName].AsTable();

			// TODO: Have a custom view for the global state
			taskInfo["GlobalState"] = new Value(globalStateTable);

			tasks.Add(
				taskName,
				new TaskDetails()
				{
					Name = taskName,
					TaskInfo = taskInfo,
					Id = this.uniqueId++,
					Children = [],
				});
		}

		// Build up the children
		foreach (var (taskName, task) in tasks)
		{
			// Build up the children set
			var runAfterClosure = task.TaskInfo["RunAfterClosureList"].AsList();

			foreach (var parent in runAfterClosure)
			{
				tasks[parent.AsString()].Children.Add(task);
			}
		}

		var graph = new List<(TaskDetails Node, IEnumerable<TaskDetails> Children)>();

		// Add each task to its own column
		foreach (var (_, task) in tasks)
		{
			graph.Add((task, task.Children));
		}

		// TODO: Should the layout be a visual aspect of the view? Yes, yes it should.
		var graphView = GraphBuilder.BuildDirectedAcyclicGraphView(
			graph,
			new Size(GraphViewer.NodeWidth, GraphViewer.NodeHeight));

		var graphNodes = BuildGraphNodes(graphView);
		return graphNodes;
	}

	private List<GraphNodeViewModel> BuildGraphNodes(
		IDictionary<TaskDetails, Point> nodePositions)
	{
		var result = new List<GraphNodeViewModel>();
		foreach (var (task, position) in nodePositions)
		{
			var toolTop = task.Name;
			var node = new GraphNodeViewModel()
			{
				Title = task.Name,
				ToolTip = toolTop,
				Id = task.Id,
				ChildNodes = task.Children.Select(value => value.Id).ToList(),
				Position = position,
			};

			result.Add(node);

			this.taskDetailsLookup.Add(
				task.Id,
				new TaskDetailsViewModel(task.TaskInfo));
		}

		return result;
	}

	private async Task<Path> GetTargetPathAsync(Path packageDirectory, string? owner)
	{
		var optionalOwnerFlag = owner is null ? string.Empty : $" -owner {owner}";
		var processInfo = new ProcessStartInfo("C:\\Program Files\\SoupBuild\\Soup\\Soup\\Soup.exe", $"target {packageDirectory}{optionalOwnerFlag}")
		{
			RedirectStandardOutput = true,
			CreateNoWindow = true,
		};
		using var process = new Process()
		{
			StartInfo = processInfo,
		};

		_ = process.Start();

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