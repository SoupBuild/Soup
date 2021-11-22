// <copyright file="App.xaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml;
using Opal;
using Opal.System;
using SoupView.View;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
namespace SoupView
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            // Register the runtime services
            Log.RegisterListener(new ConsoleTraceListener());
            LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when the application is launched normally by the end user.  Other entry points
        /// will be used such as when the application is launched to open a specific file.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(LaunchActivatedEventArgs args)
        {
            m_window = new MainWindow();

            //m_window.ExtendsContentIntoTitleBar = true;
           // m_window.SetTitleBar(new TextBlock());

            m_window.Activate();
        }

        private Window m_window;
    }
}
