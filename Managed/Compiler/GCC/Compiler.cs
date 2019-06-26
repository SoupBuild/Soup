// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler.GCC
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Threading.Tasks;

    /// <summary>
    /// The GCC compiler implementation
    /// </summary>
    public class Compiler : ICompiler
    {
        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        public string Name => "GCC";

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        public string ObjectFileExtension => "obj";

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        public string ModuleFileExtension => "gcm";

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        public string StaticLibraryFileExtension => "lib";

        /// <summary>
        /// Compile
        /// </summary>
        public Task<CompileResults> CompileAsync(CompileArguments args)
        {
            // Set the working directory to the output directory
            var workingDirectory = Path.Combine(args.RootDirectory, args.OutputDirectory);

            string compiler = "g++-m";
            var commandArgs = BuildCompilerArguments(args);

            Log.Verbose($"PWD={workingDirectory}");
            Log.Verbose($"{compiler} {commandArgs}");

            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.FileName = compiler;
                process.StartInfo.WorkingDirectory = args.RootDirectory;
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

                return Task.FromResult(new CompileResults());
            }
        }

        /// <summary>
        /// Link Library
        /// </summary>
        public Task LinkLibraryAsync(LinkerArguments args)
        {
            return Task.CompletedTask;
        }

        /// <summary>
        /// Link Executable
        /// </summary>
        public Task LinkExecutableAsync(LinkerArguments args)
        {
            return Task.CompletedTask;
        }

        private static string BuildCompilerArguments(CompileArguments args)
        {
            var commandArgs = new List<string>();

            // Set the Standard Library implementation
            // commandArgs.Add("-stdlib=libc++");

            // Set the language version
            // commandArgs.Add("-std=c++1z");

            // Enable experimental modules
            commandArgs.Add("-fmodules-ts");

            // Only compile the source
            commandArgs.Add("-c");

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
