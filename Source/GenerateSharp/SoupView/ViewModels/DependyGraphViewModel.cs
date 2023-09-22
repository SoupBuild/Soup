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
	internal class DependencyGraphViewModel : ViewModelBase
	{
		private GraphNodeViewModel? selectedNode = null;
		private ProjectDetailsViewModel? selectedProject = null;
		private string errorBarMessage = string.Empty;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNodeViewModel>>? graph = null;
		private uint uniqueId = 0;
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
			set => this.RaiseAndSetIfChanged(ref selectedNode, value);
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

		public async Task LoadProjectAsync(Path recipeFilePath)
		{
			this.uniqueId = 1;
			this.projectDetailsLookup.Clear();
			var activeGraph = new List<IList<GraphNodeViewModel>>();
			var recipeFiles = new List<(Path Path, uint Id)>()
			{
				(recipeFilePath, this.uniqueId++),
			};

			await BuildGraphAsync(recipeFiles, activeGraph);
			Graph = activeGraph;
		}

		private void NotifyError(string message)
		{
			if (Debugger.IsAttached)
				Debugger.Break();

			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private async Task BuildGraphAsync(
			IList<(Path Path, uint Id)> recipeFiles,
			IList<IList<GraphNodeViewModel>> activeGraph)
		{
			var column = new List<GraphNodeViewModel>();
			var childRecipeFiles = new List<(Path Path, uint Id)>();
			foreach (var recipeFile in recipeFiles)
			{
				var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile.Path);
				var currentChildRecipes = new List<(Path Path, uint Id)>();
				string title;
				Recipe? recipe = null;
				var packageFolder = recipeFile.Path.GetParent();

				if (loadResult.IsSuccess)
				{
					recipe = loadResult.Result;

					foreach (var dependencyType in recipe.GetDependencyTypes())
					{
						var implicitLanguage = dependencyType == Recipe.Property_Build ? "C#" : recipe.Language.Name;
						AddRecipeFiles(
							recipe.GetNamedDependencies(dependencyType),
							implicitLanguage,
							packageFolder,
							currentChildRecipes);
					}

					title = recipe.Name;
				}
				else
				{
					title = "[MISSING]";
				}

				column.Add(new GraphNodeViewModel(title, recipeFile.Id)
				{
					ChildNodes = currentChildRecipes.Select(value => value.Id).ToList(),
				});

				this.projectDetailsLookup.Add(
					recipeFile.Id,
					new ProjectDetailsViewModel(
						recipe,
						packageFolder));

				childRecipeFiles.AddRange(currentChildRecipes);
			}

			activeGraph.Add(column);

			if (childRecipeFiles.Count > 0)
			{
				await BuildGraphAsync(childRecipeFiles, activeGraph);
			}
		}

		private void AddRecipeFiles(
			IEnumerable<PackageReference> packageReferences,
			string recipeLanguage,
			Path packageFolder,
			IList<(Path Path, uint Id)> recipeFiles)
		{
			foreach (var packageReference in packageReferences)
			{
				if (packageReference.IsLocal)
				{
					var recipeFile = packageReference.Path + BuildConstants.RecipeFileName;
					if (!recipeFile.HasRoot)
					{
						recipeFile = packageFolder + recipeFile;
					}

					recipeFiles.Add((recipeFile, this.uniqueId++));
				}
				else
				{
					if (packageReference.Version == null)
						throw new InvalidOperationException("Package reference must have version");
					var packagesDirectory = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
						new Path(".soup/packages/");
					var languageRootFolder = packagesDirectory + new Path(recipeLanguage);
					var packageRootFolder = languageRootFolder + new Path(packageReference.Name);
					var packageVersionFolder = packageRootFolder +
						new Path(packageReference.Version.ToString()) +
						new Path();
					var recipeFile = packageVersionFolder + BuildConstants.RecipeFileName;

					recipeFiles.Add((recipeFile, this.uniqueId++));
				}
			}
		}
	}
}
