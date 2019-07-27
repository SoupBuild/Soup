// <copyright file="VersionCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System.Reflection;
    using System.Threading.Tasks;

    /// <summary>
    /// Version Command
    /// </summary>
    internal class VersionCommand
    {
        /// <summary>
        /// Invoke
        /// </summary>
        public Task InvokeAsync(VersionOptions options)
        {
            var version = Assembly.GetExecutingAssembly().GetName().Version;
            Log.Info($"{version.Major}.{version.Minor}.{version.Build}");
            return Task.CompletedTask;
        }
    }
}
