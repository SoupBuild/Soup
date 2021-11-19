// <copyright file="MainWindow.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using Microsoft.UI.Xaml;
using SoupView.ViewModel;
using Windows.Storage.Pickers;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
namespace SoupView.View
{
    /// <summary>
    /// An empty window that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainWindow : Window
    {
        public MainWindow()
        {
            this.InitializeComponent();

            this.Title = "Soup View";
        }

        private async void AppBarButton_Load_Click(object sender, RoutedEventArgs e)
        {
            var filePicker = new FileOpenPicker();

            // Get the current window's HWND by passing in the Window object
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle(this);

            // Associate the HWND with the file picker
            WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);

            // Use file picker like normal!
            filePicker.FileTypeFilter.Add(".toml");
            var file = await filePicker.PickSingleFileAsync();

            await ((MainPageModel)this.Root.DataContext).LoadProjectAsync(file?.Path);
        }
    }
}
