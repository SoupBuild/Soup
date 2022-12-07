// <copyright file="DependencyGraphPageModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace SoupView.ViewModel
{
	internal class DependencyGraphPageModel : Observable
	{
		private GraphNode? selectedNode = null;
		private ProjectDetailsViewModel? selectedProject = null;
		private string errorBarMessage = string.Empty;
		private bool isErrorBarOpen = false;
		private IList<IList<GraphNode>>? graph = null;
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

		public IList<IList<GraphNode>>? Graph
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

		public GraphNode? SelectedNode
		{
			get { return selectedNode; }
			set
			{
				if (value != selectedNode)
				{
					selectedNode = value;
					NotifyPropertyChanged();
					if (selectedNode != null)
					{
						SelectedProject = this.projectDetailsLookup[selectedNode.Id];
					}
					else
					{
						selectedProject = null;
					}
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

		public ProjectDetailsViewModel? SelectedProject
		{
			get { return selectedProject; }
			set
			{
				if (value != selectedProject)
				{
					selectedProject = value;
					NotifyPropertyChanged();
				}
			}
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
			if (Debugger.IsAttached)
				Debugger.Break();

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

				column.Add(new GraphNode(title, recipeFile.Id)
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
						new Path("/");
					var recipeFile = packageVersionFolder + BuildConstants.RecipeFileName;

					recipeFiles.Add((recipeFile, this.uniqueId++));
				}
			}
		}
	}
}
