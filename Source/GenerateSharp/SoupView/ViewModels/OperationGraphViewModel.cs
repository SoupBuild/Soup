// <copyright file="OperationGraphPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Threading;
using Opal;
using ReactiveUI;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace Soup.View.ViewModels
{
	public class OperationGraphViewModel : ViewModelBase
	{
		private FileSystemState fileSystemState = new FileSystemState();
		private GraphNodeViewModel? selectedNode = null;
		private OperationDetailsViewModel? selectedOperation = null;
		private string errorBarMessage = string.Empty;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNodeViewModel>>? graph = null;
		private Dictionary<uint, OperationDetailsViewModel> operationDetailsLookup = new Dictionary<uint, OperationDetailsViewModel>();

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
						SelectedOperation = this.operationDetailsLookup[selectedNode.Id];
					}
					else
					{
						SelectedOperation = null;
					}
				}
			}
		}

		public OperationDetailsViewModel? SelectedOperation
		{
			get => selectedOperation;
			set => this.RaiseAndSetIfChanged(ref selectedOperation, value);
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

						var evaluateGraphFile = soupTargetDirectory + BuildConstants.EvaluateGraphFileName;
						if (!OperationGraphManager.TryLoadState(evaluateGraphFile, fileSystemState, out var evaluateGraph))
						{
							NotifyError($"Failed to load Operation Graph: {evaluateGraphFile}");
							return null;
						}

						// Check for the optional results
						var evaluateResultsFile = soupTargetDirectory + BuildConstants.EvaluateResultsFileName;
						OperationResults? operationResults = null;
						if (OperationResultsManager.TryLoadState(evaluateResultsFile, fileSystemState, out var loadOperationResults))
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

			Graph = activeGraph;
		}

		private void NotifyError(string message)
		{
			if (Debugger.IsAttached)
				Debugger.Break();

			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private IList<IList<GraphNodeViewModel>> BuildGraph(
			OperationGraph evaluateGraph,
			OperationResults? operationResults)
		{
			this.operationDetailsLookup.Clear();

			var rootNodes = evaluateGraph.RootOperationIds.Select(value => value.value).ToList();
			var graph = evaluateGraph.Operations
				.ToDictionary(kvp => kvp.Key.value, kvp => (IList<uint>)kvp.Value.Children.Select(value => value.value).ToList());

			var graphView = GraphBuilder.BuildDirectedAcyclicGraphView(graph, rootNodes);

			var graphNodes = BuildGraphNodes(evaluateGraph, operationResults);
			var activeGraph = graphView
				.Select(column => (IList<GraphNodeViewModel>)column.Select(nodeId => graphNodes[nodeId]).ToList())
				.ToList();

			return activeGraph;
		}

		private IDictionary<uint, GraphNodeViewModel> BuildGraphNodes(
			OperationGraph evaluateGraph,
			OperationResults? operationResults)
		{
			var result = new Dictionary<uint, GraphNodeViewModel>();
			foreach (var (operationId, operation) in evaluateGraph.Operations)
			{
				var toolTop = operation.Title;
				var node = new GraphNodeViewModel(operation.Title, toolTop, operationId.value)
				{
					ChildNodes = operation.Children.Select(value => value.value).ToList(),
				};

				result.Add(operationId.value, node);

				// Check if there is a matching result
				OperationResult? operationResult = null;
				if (operationResults != null)
				{
					if (operationResults.TryFindResult(operationId, out var operationResultValue))
					{
						operationResult = operationResultValue;
					}
				}

				this.operationDetailsLookup.Add(
					operationId.value,
					new OperationDetailsViewModel(fileSystemState, operation, operationResult));
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
