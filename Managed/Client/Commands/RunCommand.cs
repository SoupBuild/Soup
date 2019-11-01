// <copyright file="RunCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System;
    using System.Diagnostics;
    using System.IO;
    using System.Threading.Tasks;

    /// <summary>
    /// Run Command
    /// </summary>
    internal class RunCommand
    {
        private LocalUserConfig _config;
        private Compiler.ICompiler _compiler;

        /// <summary>
        /// Initializes a new instance of the <see cref="RunCommand"/> class.
        /// </summary>
        public RunCommand(LocalUserConfig config, Compiler.ICompiler compiler)
        {
            _config = config;
            _compiler = compiler;
        }

        /// <summary>
        /// Invoke
        /// </summary>
        public async Task InvokeAsync(RunOptions options)
        {
            var recipePath = "./";
            var recipe = await RecipeManager.LoadFromFileAsync(recipePath);
            if (recipe == null)
            {
                Log.Error("Could not find the recipe file.");
                return;
            }

            // Ensure that this is an executable
            if (recipe.Type != RecipeType.Executable)
            {
                Log.Error("Cannot run a project not of type executable.");
                return;
            }

            // Ensure the library directory exists
            var config = "release";
            var exePath = Path.Combine(recipePath, "out", "bin", _compiler.Name, config, $"{recipe.Name}.exe");
            if (!File.Exists(exePath))
            {
                Log.Error($"The executable ({exePath}) does not exist..");
                return;
            }

            // Now build the current project
            Log.Info(string.Empty);
            Log.Info("Run Project");
            Log.Verbose(exePath);
            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.FileName = exePath;

                process.OutputDataReceived += ProcessOutputDataReceived;
                process.ErrorDataReceived += ProcessErrorDataReceived;

                process.Start();
                process.BeginOutputReadLine();
                process.BeginErrorReadLine();
                process.WaitForExit();

                if (process.ExitCode != 0)
                {
                    throw new InvalidOperationException();
                }
            }
        }

        private void ProcessOutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                Log.Info(e.Data);
            }
        }

        private void ProcessErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            Log.Error(e.Data);
        }
    }
}
