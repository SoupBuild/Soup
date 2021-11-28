// <copyright file="DependencyGraphPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.Toolkit.Mvvm.Input;
using Opal;
using Opal.System;
using Soup.Build;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;

namespace SoupView.ViewModel
{
	internal class DependencyGraphPageModel : Observable
	{
		private GraphNode selectedNode = null;
		private ProjectDetailsViewModel selectedProject = null;
		private string errorBarMessage = string.Empty;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNode>> graph = null;
		private uint uniqueId = 0;
		private Dictionary<uint, ProjectDetailsViewModel> projectDetailsLookup = new Dictionary<uint, ProjectDetailsViewModel>();

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

		public GraphNode SelectedNode
		{
			get { return selectedNode; }
			set
			{
				if (value != selectedNode)
				{
					selectedNode = value;
					NotifyPropertyChanged();
					SelectedProject = this.projectDetailsLookup[selectedNode.Id];
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

		public bool IsDetailsPaneOpen
		{
			get { return SelectedProject != null; }
		}

		public ProjectDetailsViewModel SelectedProject
		{
			get { return selectedProject; }
			set
			{
				if (value != selectedProject)
				{
					selectedProject = value;
					NotifyPropertyChanged();
					NotifyPropertyChanged("IsDetailsPaneOpen");
				}
			}
		}

		public ICommand ClickNode => new RelayCommand(this.OnClickNode);

		private void OnClickNode()
		{

		}

		public async Task LoadProjectAsync(Path recipeFilePath)
		{
			this.uniqueId = 1;
			this.projectDetailsLookup.Clear();
			var activeGraph = new List<IList<GraphNode>>();
			var recipeFiles = new List<(Path Path, uint Id)>()
			{
				(recipeFilePath, this.uniqueId++),
			};
			await BuildGraphAsync(recipeFiles, activeGraph);
			Graph = activeGraph;
		}

		private void NotifyError(string message)
		{
			ErrorBarMessage = message;
			IsErrorBarOpen = true;
		}

		private async Task BuildGraphAsync(IList<(Path Path, uint Id)> recipeFiles, IList<IList<GraphNode>> activeGraph)
		{
			var column = new List<GraphNode>();
			var childRecipeFiles = new List<(Path Path, uint Id)>();
			foreach (var recipeFile in recipeFiles)
			{
				var loadResult = await RecipeExtensions.TryLoadRecipeFromFileAsync(recipeFile.Path);
				if (!loadResult.IsSuccess)
				{
					NotifyError($"Failed to load Recipe file: {recipeFile.Path}");
				}

				var packageFolder = recipeFile.Path.GetParent();
				var recipe = loadResult.Result;

				var currentChildRecipes = new List<(Path Path, uint Id)>();
				if (recipe.HasBuildDependencies)
					AddRecipeFiles(recipe.BuildDependencies, recipe.Language, packageFolder, currentChildRecipes);
				if (recipe.HasTestDependencies)
					AddRecipeFiles(recipe.TestDependencies, recipe.Language, packageFolder, currentChildRecipes);
				if (recipe.HasRuntimeDependencies)
					AddRecipeFiles(recipe.RuntimeDependencies, recipe.Language, packageFolder, currentChildRecipes);

				column.Add(new GraphNode(recipe.Name, recipeFile.Id)
				{
					ChildNodes = currentChildRecipes.Select(value => value.Id).ToList(),
				});

				this.projectDetailsLookup.Add(
					recipeFile.Id,
					new ProjectDetailsViewModel(
						recipe.Name,
						recipe.Version.ToString(),
						packageFolder.ToString()));

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
					var packagesDirectory = LifetimeManager.Get<IFileSystem>().GetUserProfileDirectory() +
						new Path(".soup/packages/");
					var languageRootFolder = packagesDirectory + new Path(recipeLanguage);
					var packageRootFolder = languageRootFolder + new Path(packageReference.GetName);
					var packageVersionFolder = packageRootFolder + new Path(packageReference.Version.ToString()) + new Path("/");
					var recipeFile = packageVersionFolder + BuildConstants.RecipeFileName;

					recipeFiles.Add((recipeFile, this.uniqueId++));
				}
			}
		}
	}
}
