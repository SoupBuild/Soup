// <copyright file="DependencyGraphViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Threading;
using Opal;
using Opal.System;
using ReactiveUI;
using Soup.Build;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace Soup.View.ViewModels
{
	public class DependencyGraphViewModel : ViewModelBase
	{
		private GraphNodeViewModel? selectedNode = null;
		private ProjectDetailsViewModel? selectedProject = null;
		private string errorBarMessage = string.Empty;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNodeViewModel>>? graph = null;
		private Dictionary<uint, ProjectDetailsViewModel> projectDetailsLookup = new Dictionary<uint, ProjectDetailsViewModel>();

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
						SelectedProject = this.projectDetailsLookup[selectedNode.Id];
					}
					else
					{
						SelectedProject = null;
					}
				}
			}
		}

		public bool IsErrorBarOpen
		{
			get => isErrorBarOpen;
			set => this.RaiseAndSetIfChanged(ref isErrorBarOpen, value);
		}

		public ProjectDetailsViewModel? SelectedProject
		{
			get => selectedProject;
			set => this.RaiseAndSetIfChanged(ref selectedProject, value);
		}

		public DependencyGraphViewModel()
		{
		}

		public async Task LoadProjectAsync(Path recipeFilePath)
		{
			var activeGraph = await Task.Run(() =>
			{
				this.projectDetailsLookup.Clear();

				var workingDirectory = recipeFilePath.GetParent();
				var packageProvider = SoupTools.LoadBuildGraph(workingDirectory);

				var activeGraph = BuildGraph(packageProvider);
				return activeGraph;
			});

			var rootNode = activeGraph.FirstOrDefault()?.FirstOrDefault();

			Graph = activeGraph;
			SelectedNode = rootNode;
		}

		private void NotifyError(string message)
		{
			if (Debugger.IsAttached)
				Debugger.Break();

			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private IList<IList<GraphNodeViewModel>> BuildGraph(
			PackageProvider packageProvider)
		{
			this.projectDetailsLookup.Clear();

			var rootNodes = new List<uint>()
			{
				(uint)packageProvider.RootPackageGraphId,
			};
			var graph = packageProvider.PackageLookup
				.ToDictionary(
					kvp => (uint)kvp.Key,
					kvp => GetChildren(kvp.Value.Dependencies, packageProvider));

			var graphView = GraphBuilder.BuildDirectedAcyclicGraphView(graph, rootNodes);

			var graphNodes = BuildGraphNodes(packageProvider);
			var activeGraph = graphView
				.Select(column => (IList<GraphNodeViewModel>)column.Select(nodeId => graphNodes[nodeId]).ToList())
				.ToList();

			return activeGraph;
		}

		private IDictionary<uint, GraphNodeViewModel> BuildGraphNodes(
			PackageProvider packageProvider)
		{
			var result = new Dictionary<uint, GraphNodeViewModel>();
			foreach (var (packageId, package) in packageProvider.PackageLookup)
			{
				string title = package.Name;
				string toolTip = package.PackageRoot;
				var packageFolder = new Path(package.PackageRoot);

				var node = new GraphNodeViewModel(title, toolTip, (uint)packageId)
				{
					ChildNodes = GetChildren(package.Dependencies, packageProvider),
				};

				result.Add((uint)packageId, node);

				this.projectDetailsLookup.Add(
					(uint)packageId,
					new ProjectDetailsViewModel(
						package.Name,
						packageFolder));
			}

			return result;
		}

		private static IList<uint> GetChildren(
			IDictionary<string, IList<PackageChildInfo>> dependencies,
			PackageProvider packageProvider)
		{
			var result = new List<uint>();
			foreach (var (dependencyType, children) in dependencies)
			{
				foreach (var child in children)
				{
					if (child.IsSubGraph)
					{
						var subGraph = packageProvider.GetPackageGraph(child.PackageGraphId ??
							throw new InvalidOperationException("SubGraph child does not have package graph id"));

						// TODO: result.Add((uint)subGraph.RootPackageId);
					}
					else
					{
						result.Add((uint?)child.PackageId ??
							throw new InvalidOperationException("Package child does not have package id"));
					}
				}
			}

			return result;
		}
	}
}
