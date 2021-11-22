// <copyright file="MainWindowModel.cs" company="Soup">
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
                    _ = DependencyGraph.LoadProjectAsync(recipeFile);
                    _ = OperationGraph.LoadProjectAsync(recipeFile);
                }
            }
        }

        public DependencyGraphPageModel DependencyGraph { get; private set; } = new DependencyGraphPageModel();
        public OperationGraphPageModel OperationGraph { get; private set; } = new OperationGraphPageModel();
    }
}
