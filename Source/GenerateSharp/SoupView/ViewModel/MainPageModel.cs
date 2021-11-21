// <copyright file="MainPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace SoupView.ViewModel
{
    internal class MainPageModel : Observable
    {
        private string errorBarMessage = string.Empty;
        private bool isErrorBarOpen = false;
        private IList<IList<GraphNode>> graph = null;

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

                var evaluateGraphFile = soupTargetDirectory + BuildConstants.GenerateEvaluateOperationGraphFileName;
                var fileSystemState = new FileSystemState();
                if (!OperationGraphManager.TryLoadState(evaluateGraphFile, fileSystemState, out var evaluateGraph))
                {
                    NotifyError($"Failed to load Operation Graph: {evaluateGraphFile}");
                    return;
                }

                Graph = BuildGraph(evaluateGraph);
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

        private IList<IList<GraphNode>> BuildGraph(OperationGraph evaluateGraph)
        {
            var activeIds = evaluateGraph.GetRootOperationIds();
            var activeGraph = new List<IList<GraphNode>>();
            var knownIds = new HashSet<OperationId>();
            BuildGraphColumn(evaluateGraph, activeGraph, activeIds, knownIds);

            return activeGraph;
        }

        private void BuildGraphColumn(
            OperationGraph evaluateGraph,
            IList<IList<GraphNode>> activeGraph,
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
                BuildGraphColumn(evaluateGraph, activeGraph, nextIds, knownIds);
            }

            // Build up all the nodes at this level that have not already been added
            var column = new List<GraphNode>();
            foreach (var operationId in activeIds)
            {
                if (!knownIds.Contains(operationId))
                {
                    var operation = evaluateGraph.GetOperationInfo(operationId);

                    var node = new GraphNode(operation.Title, operationId.value)
                    {
                        ChildNodes = operation.Children.Select(value => value.value).ToList(),
                    };

                    knownIds.Add(operationId);
                    column.Add(node);
                }
            }

            // Add the new column at the start
            activeGraph.Insert(0, column);
        }

        private async Task<Path> GetTargetPathAsync(Path packageDirectory)
        {
            var processInfo = new ProcessStartInfo("soup.exe", $"target {packageDirectory}")
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
