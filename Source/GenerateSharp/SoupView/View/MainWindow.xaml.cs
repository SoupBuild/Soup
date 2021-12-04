// <copyright file="MainWindow.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Threading.Tasks;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Opal;
using SoupView.ViewModel;
using Windows.Storage.Pickers;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
namespace SoupView.View
{
	/// <summary>
	/// An main window that can be used on its own or navigated to within a Frame.
	/// </summary>
	public sealed partial class MainWindow : Window
	{
		private MainWindowModel ViewModel => (MainWindowModel)this.Root.DataContext;

		public MainWindow()
		{
			this.InitializeComponent();

			this.ExtendsContentIntoTitleBar = true;
			this.SetTitleBar(AppTitleBar);

			this.Title = "Soup View";
			LoadIcon("Assets/Soup.ico");

			Navigation.SelectedItem = DependencyGraphNavigationItem;
		}

		private void LoadIcon(string iconName)
		{
			// Get the Window's HWND
			var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);
			var iconHandle = PInvoke.User32.LoadImage(
				IntPtr.Zero,
				iconName,
				PInvoke.User32.ImageType.IMAGE_ICON,
				32,
				32,
				PInvoke.User32.LoadImageFlags.LR_LOADFROMFILE);

			PInvoke.User32.SendMessage(hwnd, PInvoke.User32.WindowMessage.WM_SETICON, (IntPtr)0, iconHandle);
		}

		private async void NavigationView_SelectionChanged(NavigationView sender, NavigationViewSelectionChangedEventArgs args)
		{
			if (args.IsSettingsSelected)
			{
				ContentFrame.Navigate(typeof(SettingsPage));
			}
			else
			{
				var selectedItem = (NavigationViewItem)args.SelectedItem;
				if (selectedItem == DependencyGraphNavigationItem)
				{
					ContentFrame.Navigate(typeof(DependencyGraphPage), ViewModel.DependencyGraph);
				}
				else if (selectedItem == TaskGraphNavigationItem)
				{
					ContentFrame.Navigate(typeof(TaskGraphPage), ViewModel.TaskGraph);
				}
				else if (selectedItem == OperationGraphNavigationItem)
				{
					ContentFrame.Navigate(typeof(OperationGraphPage), ViewModel.OperationGraph);
				}
				else if (selectedItem == OpenFileNavigationItem)
				{
					await LoadFileAsync();
					this.Navigation.SelectedItem = DependencyGraphNavigationItem;
				}
				else
				{
					throw new InvalidOperationException("Unknown selection.");
				}
			}
		}

		private async Task LoadFileAsync()
		{
			var filePicker = new FileOpenPicker();

			// Get the current window's HWND by passing in the Window object
			var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);

			// Associate the HWND with the file picker
			WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);

			// Use file picker like normal!
			filePicker.FileTypeFilter.Add(".toml");
			var file = await filePicker.PickSingleFileAsync();

			if (file != null)
			{
				ViewModel.RecipeFile = new Path(file.Path);
			}
		}
	}
}
