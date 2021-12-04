// <copyright file="MainWindowModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;

namespace SoupView.ViewModel
{
	internal class MainWindowModel : Observable
	{
		private Path recipeFile = new Path();

		public Path RecipeFile
		{
			get { return recipeFile; }
			set
			{
				if (value != recipeFile)
				{
					recipeFile = value;
					NotifyPropertyChanged();
					NotifyPropertyChanged("Title");
					_ = DependencyGraph.LoadProjectAsync(recipeFile);
					_ = TaskGraph.LoadProjectAsync(recipeFile);
					_ = OperationGraph.LoadProjectAsync(recipeFile);
				}
			}
		}

		public string Title
		{
			get
			{
				if (RecipeFile == null || RecipeFile.IsEmpty)
				{
					return string.Empty;
				}
				else
				{


					return RecipeFile.ToString();
				}
			}
		}

		public DependencyGraphPageModel DependencyGraph { get; private set; } = new DependencyGraphPageModel();
		public TaskGraphPageModel TaskGraph { get; private set; } = new TaskGraphPageModel();
		public OperationGraphPageModel OperationGraph { get; private set; } = new OperationGraphPageModel();
	}
}
