// <copyright file="OperationGraphPageModel.cs" company="Soup">
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

namespace Soup.View.ViewModels;

public class OperationGraphViewModel : ContentPaneViewModel
{
	private readonly FileSystemState fileSystemState = new FileSystemState();
	private GraphNodeViewModel? selectedNode;
	private OperationDetailsViewModel? selectedOperation;
	private IList<GraphNodeViewModel>? graph;
	private readonly Dictionary<uint, OperationDetailsViewModel> operationDetailsLookup = [];

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
				this.SelectedOperation = this.selectedNode is not null ? this.operationDetailsLookup[this.selectedNode.Id] : null;
			}
		}
	}

	public OperationDetailsViewModel? SelectedOperation
	{
		get => this.selectedOperation;
		set => this.RaiseAndSetIfChanged(ref this.selectedOperation, value);
	}

	public async Task LoadProjectAsync(Path? packageFolder)
	{
		this.Graph = null;

		var activeGraph = await Task.Run(async () =>
		{
			if (packageFolder is not null)
			{
				var recipeFile = packageFolder + BuildConstants.RecipeFileName;
				var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile);
				if (loadResult.IsSuccess)
				{
					var targetPath = await GetTargetPathAsync(packageFolder);

					var soupTargetDirectory = targetPath + new Path("./.soup/");

					var evaluateGraphFile = soupTargetDirectory + BuildConstants.EvaluateGraphFileName;
					if (!OperationGraphManager.TryLoadState(evaluateGraphFile, this.fileSystemState, out var evaluateGraph))
					{
						NotifyError($"Failed to load Operation Graph: {evaluateGraphFile}");
						return null;
					}

					// Check for the optional results
					var evaluateResultsFile = soupTargetDirectory + BuildConstants.EvaluateResultsFileName;
					OperationResults? operationResults = null;
					if (OperationResultsManager.TryLoadState(evaluateResultsFile, this.fileSystemState, out var loadOperationResults))
					{
						operationResults = loadOperationResults;
					}

					return BuildGraph(evaluateGraph, operationResults);
				}
				else
				{
					NotifyError($"Failed to load Recipe file: {packageFolder}");
					return null;
				}
			}

			return null;
		});

		this.Graph = activeGraph;
	}

	private List<GraphNodeViewModel> BuildGraph(
		OperationGraph evaluateGraph,
		OperationResults? operationResults)
	{
		this.operationDetailsLookup.Clear();

		var graph = evaluateGraph.Operations
			.Select(value => (value.Value, value.Value.Children.Select(value => evaluateGraph.Operations[value])));

		// TODO: Should the layout be a visual aspect of the view? Yes, yes it should.
		var graphView = GraphBuilder.BuildDirectedAcyclicGraphView(
			graph,
			new Size(GraphViewer.NodeWidth, GraphViewer.NodeHeight));

		var graphNodes = BuildGraphNodes(graphView, operationResults);

		return graphNodes;
	}

	private List<GraphNodeViewModel> BuildGraphNodes(
		IDictionary<OperationInfo, Point> nodePositions,
		OperationResults? operationResults)
	{
		var result = new List<GraphNodeViewModel>();
		foreach (var (operation, position) in nodePositions)
		{
			var toolTop = operation.Title;
			var node = new GraphNodeViewModel()
			{
				Title = operation.Title,
				ToolTip = toolTop,
				Id = operation.Id.Value,
				ChildNodes = operation.Children.Select(value => value.Value).ToList(),
				Position = position,
			};

			result.Add(node);

			// Check if there is a matching result
			OperationResult? operationResult = null;
			if (operationResults != null)
			{
				if (operationResults.TryFindResult(operation.Id, out var operationResultValue))
				{
					operationResult = operationResultValue;
				}
			}

			this.operationDetailsLookup.Add(
				operation.Id.Value,
				new OperationDetailsViewModel(this.fileSystemState, operation, operationResult));
		}

		return result;
	}

	private async Task<Path> GetTargetPathAsync(Path packageDirectory)
	{
		var processInfo = new ProcessStartInfo("C:\\Program Files\\SoupBuild\\Soup\\Soup\\Soup.exe", $"target {packageDirectory}")
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