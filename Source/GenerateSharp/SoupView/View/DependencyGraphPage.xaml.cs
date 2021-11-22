// <copyright file="DependencyGraphPage.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using SoupView.ViewModel;

namespace SoupView.View
{
    /// <summary>
    /// The dependency graph page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class DependencyGraphPage : Page
    {
        public DependencyGraphPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            DataContext = (DependencyGraphPageModel)e.Parameter;
        }
    }
}
