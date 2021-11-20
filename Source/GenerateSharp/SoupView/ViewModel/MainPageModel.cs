// <copyright file="MainPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;

namespace SoupView.ViewModel
{
    internal class MainPageModel : Observable
    {
        private string errorBarMessage = string.Empty;
        private bool isErrorBarOpen = false;
        private IList<IList<GraphNode>> graph = new List<IList<GraphNode>>()
        {
            new List<GraphNode>()
            {
                new GraphNode("1", 1)
                {
                    ChildNodes = new List<uint>() { 5, 6, },
                },
                new GraphNode("2", 2)
                {
                    ChildNodes = new List<uint>() { 6, },
                },
                new GraphNode("3", 3)
                {
                    ChildNodes = new List<uint>() { 6, },
                },
                new GraphNode("4", 4)
                {
                    ChildNodes = new List<uint>() { 6, },
                },
            },
            new List<GraphNode>()
            {
                new GraphNode("5", 5)
                {
                    ChildNodes = new List<uint>() { 8, },
                },
                new GraphNode("6", 6)
                {
                    ChildNodes = new List<uint>() { 8, },
                },
                new GraphNode("7", 7)
                {
                    ChildNodes = new List<uint>() { 9, },
                },
                null,
            },
            new List<GraphNode>()
            {
                null,
                new GraphNode("8", 8),
                new GraphNode("9", 9),
                null,
            },
        };

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

                var rootIds = evaluateGraph.GetRootOperationIds();
                var graph = new List<IList<GraphNode>>();

                var column = new List<GraphNode>();
                foreach (var rootId in rootIds)
                {
                    var operation = evaluateGraph.GetOperationInfo(rootId);
                    column.Add(new GraphNode(operation.Title, rootId.value));
                }

                graph.Add(column);

                Graph = graph;
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

        private async Task<Path> GetTargetPathAsync(Path packageDirectory)
        {
            var processInfo = new ProcessStartInfo("soup.exe", $"target {packageDirectory}")
            {
                RedirectStandardOutput = true,
            };
            var process = Process.Start(processInfo);
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
