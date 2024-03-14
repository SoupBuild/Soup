// <copyright file="DependencyGraphViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using GraphShape;
using Opal;
using ReactiveUI;
using Soup.View.Views;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace Soup.View.ViewModels;

public class DependencyGraphViewModel : ContentPaneViewModel
{
	private GraphNodeViewModel? selectedNode;
	private ProjectDetailsViewModel? selectedProject;
	private IList<GraphNodeViewModel>? graph;
	private readonly Dictionary<uint, ProjectDetailsViewModel> projectDetailsLookup = [];

	public IList<GraphNodeViewModel>? Graph
	{
		get => graph;
		private set => this.RaiseAndSetIfChanged(ref graph, value);
	}

	public GraphNodeViewModel? SelectedNode
	{
		get => selectedNode;
		set
		{
			if (this.CheckRaiseAndSetIfChanged(ref selectedNode, value))
			{
				SelectedProject = selectedNode is not null ? this.projectDetailsLookup[selectedNode.Id] : null;
			}
		}
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

		var rootNode = activeGraph.FirstOrDefault();

		Graph = activeGraph;
		SelectedNode = rootNode;
	}

	private List<GraphNodeViewModel> BuildGraph(
		PackageProvider packageProvider)
	{
		this.projectDetailsLookup.Clear();

		var currentGraphSet = GetCurrentGraphSet(packageProvider);

		// Filter to only the current sub graph
		var graph = packageProvider.PackageLookup
			.Where(value => currentGraphSet.Contains(value.Key))
			.Select(value => (value.Value, (IEnumerable<PackageInfo>)GetChildren(value.Value.Dependencies, packageProvider)));

		// TODO: Should the layout be a visual aspect of the view? Yes, yes it should.
		var graphView = GraphBuilder.BuildDirectedAcyclicGraphView(
			graph,
			new Size(GraphViewer.NodeWidth, GraphViewer.NodeHeight));

		var graphNodes = BuildGraphNodes(graphView, packageProvider);

		return graphNodes;
	}

	private static HashSet<int> GetCurrentGraphSet(PackageProvider packageProvider)
	{
		var result = new HashSet<int>();

		var activeNodes = new Stack<int>();
		activeNodes.Push(packageProvider.RootPackageGraphId);

		while (activeNodes.Count > 0)
		{
			var currentNodeId = activeNodes.Pop();
			_ = result.Add(currentNodeId);

			foreach (var child in GetChildren(packageProvider.GetPackageInfo(currentNodeId).Dependencies, packageProvider))
			{
				if (!result.Contains(child.Id))
					activeNodes.Push(child.Id);
			}
		}

		return result;
	}

	private List<GraphNodeViewModel> BuildGraphNodes(
		IDictionary<PackageInfo, Point> nodePositions,
		PackageProvider packageProvider)
	{
		var result = new List<GraphNodeViewModel>();
		foreach (var (package, position) in nodePositions)
		{
			string title = package.Name;
			string toolTip = package.PackageRoot;
			var packageFolder = new Path(package.PackageRoot);

			var node = new GraphNodeViewModel()
			{
				Title = title,
				ToolTip = toolTip,
				Id = (uint)package.Id,
				ChildNodes = GetChildren(package.Dependencies, packageProvider).Select(value => (uint)value.Id).ToList(),
				Position = nodePositions[package],
			};

			result.Add(node);

			this.projectDetailsLookup.Add(
				(uint)package.Id,
				new ProjectDetailsViewModel(
					package.Name,
					packageFolder));
		}

		return result;
	}

	private static List<PackageInfo> GetChildren(
		IDictionary<string, IList<PackageChildInfo>> dependencies,
		PackageProvider packageProvider)
	{
		var result = new List<PackageInfo>();
		foreach (var (dependencyType, children) in dependencies)
		{
			foreach (var child in children)
			{
				if (child.IsSubGraph)
				{
					var subGraph = packageProvider.GetPackageGraph(child.PackageGraphId ??
						throw new InvalidOperationException("SubGraph child does not have package graph id"));

					// TODO: result.Add(packageProvider.GetPackageInfo(subGraph.RootPackageId));
				}
				else
				{
					result.Add(packageProvider.GetPackageInfo(child.PackageId ??
						throw new InvalidOperationException("Package child does not have package id")));
				}
			}
		}

		return result;
	}
}