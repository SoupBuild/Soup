// <copyright file="TaskGraphPage.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using SoupView.ViewModel;

namespace SoupView.View
{
    /// <summary>
    /// The task graph page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class TaskGraphPage : Page
    {
        public TaskGraphPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            DataContext = (TaskGraphPageModel)e.Parameter;
        }
    }
}
