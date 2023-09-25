// <copyright file="OperationGraphPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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

		public async Task LoadProjectAsync(Path recipeFilePath)
		{
			var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFilePath);
			if (loadResult.IsSuccess)
			{
				var packageDirectory = recipeFilePath.GetParent();
				var targetPath = await GetTargetPathAsync(packageDirectory);

				var soupTargetDirectory = targetPath + new Path(".soup/");

				var evaluateGraphFile = soupTargetDirectory + BuildConstants.EvaluateGraphFileName;
				var fileSystemState = new FileSystemState();
				if (!OperationGraphManager.TryLoadState(evaluateGraphFile, fileSystemState, out var evaluateGraph))
				{
					NotifyError($"Failed to load Operation Graph: {evaluateGraphFile}");
					return;
				}

				// Check for the optional results
				var evaluateResultsFile = soupTargetDirectory + BuildConstants.EvaluateResultsFileName;
				OperationResults? operationResults = null;
				if (OperationResultsManager.TryLoadState(evaluateResultsFile, fileSystemState, out var loadOperationResults))
				{
					operationResults = loadOperationResults;
				}

				Graph = BuildGraph(fileSystemState, evaluateGraph, operationResults);
			}
			else
			{
				NotifyError($"Failed to load Recipe file: {recipeFilePath}");
			}
		}

		private void NotifyError(string message)
		{
			if (Debugger.IsAttached)
				Debugger.Break();

			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private IList<IList<GraphNodeViewModel>> BuildGraph(
			FileSystemState fileSystemState,
			OperationGraph evaluateGraph,
			OperationResults? operationResults)
		{
			this.operationDetailsLookup.Clear();
			var activeIds = evaluateGraph.RootOperationIds;
			var activeGraph = new List<IList<GraphNodeViewModel>>();
			var knownIds = new HashSet<OperationId>();
			BuildGraphColumn(
				fileSystemState,
				evaluateGraph,
				operationResults,
				activeGraph,
				activeIds,
				knownIds);

			return activeGraph;
		}

		private void BuildGraphColumn(
			FileSystemState fileSystemState,
			OperationGraph evaluateGraph,
			OperationResults? operationResults,
			IList<IList<GraphNodeViewModel>> activeGraph,
			IList<OperationId> activeIds,
			HashSet<OperationId> knownIds)
		{
			// Build up the total set of nodes in the next level
			var nextIds = new List<OperationId>();
			foreach (var operationId in activeIds)
			{
				var operation = evaluateGraph.GetOperationInfo(operationId);
				foreach (var childId in operation.Children)
				{
					nextIds.Add(childId);
				}
			}

			// Find the depest level first
			if (nextIds.Count > 0)
			{
				BuildGraphColumn(
					fileSystemState,
					evaluateGraph,
					operationResults,
					activeGraph,
					nextIds,
					knownIds);
			}

			// Build up all the nodes at this level that have not already been added
			var column = new List<GraphNodeViewModel>();
			foreach (var operationId in activeIds)
			{
				if (!knownIds.Contains(operationId))
				{
					var operation = evaluateGraph.GetOperationInfo(operationId);
					var toolTop = string.Empty;
					var node = new GraphNodeViewModel(operation.Title, toolTop, operationId.value)
					{
						ChildNodes = operation.Children.Select(value => value.value).ToList(),
					};

					knownIds.Add(operationId);
					column.Add(node);

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
			}

			// Add the new column at the start
			activeGraph.Insert(0, column);
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
