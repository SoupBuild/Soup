// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler.Clang
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Threading.Tasks;

    /// <summary>
    /// The Clang compiler implementation
    /// </summary>
    public class Compiler : ICompiler
    {
        /// <summary>
        /// Compile
        /// </summary>
        public Task CompileAsync(CompilerArguments args)
        {
            // Set the working directory to the output directory
            var workingDirectory = Path.Combine(args.RootDirectory, args.OutputDirectory);

            string compiler = "/usr/bin/clang++";
            var commandArgs = BuildCompilerArguments(workingDirectory, args);

            Log.Info($"{compiler} {commandArgs}");
            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.FileName = compiler;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = commandArgs;
                process.Start();

                while (!process.StandardOutput.EndOfStream)
                {
                    string line = process.StandardOutput.ReadLine();
                    ProcessLine(line);
                }

                process.WaitForExit();

                if (process.ExitCode != 0)
                {
                    throw new InvalidOperationException();
                }

                return Task.CompletedTask;
            }
        }

        /// <summary>
        /// Link library
        /// </summary>
        public Task LinkLibraryAsync(LinkerArguments args)
        {
            return Task.CompletedTask;
        }

        /// <summary>
        /// Link executable
        /// </summary>
        public Task LinkExecutableAsync(LinkerArguments args)
        {
            return Task.CompletedTask;
        }

        private static string BuildCompilerArguments(string workingDirectory, CompilerArguments args)
        {
            var commandArgs = new List<string>();

            // Only run preprocess, compile, and assemble steps
            commandArgs.Add("-c");

            // Set the Standard Library implementation
            commandArgs.Add("-stdlib=libc++");

            // Set the language version
            commandArgs.Add("-std=c++1z");

            // Enable experimental modules
            commandArgs.Add("-fmodules-ts");

            // Output object
            commandArgs.Add("-o");
            commandArgs.Add("MultiVersion.ifc");

            // Lastly add the file
            commandArgs.AddRange(args.SourceFiles);

            return string.Join(" ", commandArgs);
        }

        private static void ProcessLine(string line)
        {
            Log.Info(line);
        }
    }
}
