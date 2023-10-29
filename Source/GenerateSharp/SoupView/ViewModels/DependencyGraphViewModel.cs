// <copyright file="DependencyGraphViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
			this.projectDetailsLookup.Clear();

			var workingDirectory = recipeFilePath.GetParent();
			var packageProvider = SoupTools.LoadBuildGraph(workingDirectory);

			var activeGraph = new List<IList<GraphNodeViewModel>>();
			var packages = new List<int>()
			{
				packageProvider.GetRootPackageGraph().RootPackageId,
			};

			var rootColumn = await BuildGraphAsync(packages, activeGraph, packageProvider);
			var rootNode = rootColumn.FirstOrDefault();

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

		private async Task<IList<GraphNodeViewModel>> BuildGraphAsync(
			IList<int> packages,
			IList<IList<GraphNodeViewModel>> activeGraph,
			PackageProvider packageProvider)
		{
			var column = new List<GraphNodeViewModel>();
			var childPackages = new List<int>();
			foreach (var packageId in packages)
			{
				var package = packageProvider.GetPackageInfo(packageId);

				var currentChildPackages = new List<int>();
				string title;
				string toolTip = package.PackageRoot;
				Recipe? recipe = null;
				var packageFolder = new Path(package.PackageRoot);

				foreach (var dependencyType in package.Dependencies)
				{
					AddRecipeFiles(
						dependencyType.Value,
						packageProvider,
						currentChildPackages);
				}

				title = package.Name;

				column.Add(new GraphNodeViewModel(title, toolTip, (uint)packageId)
				{
					ChildNodes = currentChildPackages.Select(value => (uint)value).ToList(),
				});

				this.projectDetailsLookup.Add(
					(uint)packageId,
					new ProjectDetailsViewModel(
						recipe,
						packageFolder));

				childPackages.AddRange(currentChildPackages);
			}

			activeGraph.Add(column);

			if (childPackages.Count > 0)
			{
				_ = await BuildGraphAsync(childPackages, activeGraph, packageProvider);
			}

			return column;
		}

		private void AddRecipeFiles(
			IEnumerable<PackageChildInfo> children,
			PackageProvider packageProvider,
			IList<int> packages)
		{
			foreach (var child in children)
			{
				if (child.IsSubGraph)
				{
					var subGraph = packageProvider.GetPackageGraph(child.PackageGraphId ??
						throw new InvalidOperationException("SubGraph child does not have package graph id"));

					packages.Add(subGraph.RootPackageId);
				}
				else
				{
					packages.Add(child.PackageId ??
						throw new InvalidOperationException("Package child does not have package id"));
				}
			}
		}
	}
}
