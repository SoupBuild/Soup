// <copyright file="OperationGraphPage.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;
using SoupView.ViewModel;

namespace SoupView.View
{
	/// <summary>
	/// The operation graph page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class OperationGraphPage : Page
	{
		public OperationGraphPage()
		{
			this.InitializeComponent();
		}

		protected override void OnNavigatedTo(NavigationEventArgs e)
		{
			DataContext = (OperationGraphPageModel)e.Parameter;
		}
	}
}
