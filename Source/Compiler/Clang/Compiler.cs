// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler.Clang
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.IO;
    using System.Linq;
    using System.Threading.Tasks;

    /// <summary>
    /// The MSVC compiler implementation
    /// </summary>
    public class Compiler : ICompiler
    {
        private static string ToolsPath => @"C:\Program Files\LLVM";

        /// <summary>
        /// Compile
        /// </summary>
        public Task CompileAsync(CompilerArguments args)
        {
            // Set the working directory to the output directory
            var workingDirectory = Path.Combine(args.RootDirectory, args.OutputDirectory);

            string compiler = Path.Combine(ToolsPath, @"bin\clang++.exe");
            var commandArgs = BuildCompilerArguments(args, workingDirectory);

            Log.Verbose($"PWD={workingDirectory}");
            Log.Verbose($"{compiler} {commandArgs}");

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
            // Set the working directory to the output directory
            var workingDirectory = args.RootDirectory;

            string linker = Path.Combine(ToolsPath, @"bin\llvm-ar.exe");
            var linkerArgs = BuildLinkerLibraryArguments(args);

            Log.Verbose($"PWD={workingDirectory}");
            Log.Verbose($"{linker} {linkerArgs}");

            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.FileName = linker;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = linkerArgs;
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
        /// Link Executable
        /// </summary>
        public Task LinkExecutableAsync(LinkerArguments args)
        {
            // Set the working directory to the output directory
            var workingDirectory = args.RootDirectory;

            string linker = Path.Combine(ToolsPath, @"bin\clang++.exe");
            var linkerArgs = BuildLinkerExecutableArguments(args);

            Log.Verbose($"PWD={workingDirectory}");
            Log.Verbose($"{linker} {linkerArgs}");

            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.FileName = linker;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = linkerArgs;
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

        private static string BuildCompilerArguments(CompilerArguments args, string workingDirectory)
        {
            var commandArgs = new List<string>();

            // Set the language standard
            switch (args.Standard)
            {
                case LanguageStandard.CPP11:
                    commandArgs.Add("-std=c++11");
                    break;
                case LanguageStandard.CPP14:
                    commandArgs.Add("-std=c++14");
                    break;
                case LanguageStandard.CPP17:
                case LanguageStandard.Latest:
                    commandArgs.Add("-std=c++17");
                    break;
                default:
                    throw new NotSupportedException("Unknown language standard.");
            }

            // Set the include paths
            foreach (var directory in args.IncludeDirectories)
            {
                commandArgs.Add($"-I\"{directory}\"");
            }

            // Set the preprocessor definitions
            foreach (var definition in args.PreprocessorDefinitions)
            {
                commandArgs.Add($"-D{definition}");
            }

            // Ignore Standard Include Paths to prevent pulling in accidental headers
            // commandArgs.Add("-X");

            // Add in the std include paths

            // Calculate object output file
            var rootPath = Path.GetRelativePath(workingDirectory, args.RootDirectory);

            // Enable c++ exceptions
            // commandArgs.Add("-EHs");

            // Enable experimental features
            if (args.Standard == LanguageStandard.Latest)
            {
                commandArgs.Add("-fmodules-ts");
            }

            // Add the module references
            foreach (var module in args.Modules)
            {
                commandArgs.Add($"-fmodule-file=\"{Path.Combine(rootPath, module.Replace("ifc", "pcm"))}\"");
            }

            if (args.ExportModule)
            {
                commandArgs.Add("--precompile");

                // There must be only one source file
                if (args.SourceFiles.Count != 1)
                {
                    throw new ArgumentException("Export module expects only one source file.");
                }

                // Place the ifc in the output directory
                var sourceFile = args.SourceFiles[0];
                var outputFile = $"{Path.GetFileNameWithoutExtension(sourceFile)}.pcm";
                commandArgs.AddRange(new string[] { "-o", outputFile });
            }
            else
            {
                // Only run preprocessor, compile and assemble
                commandArgs.Add("-c");
            }

            // Lastly add the file
            commandArgs.AddRange(args.SourceFiles.Select(file => Path.Combine(rootPath, file)));

            return string.Join(" ", commandArgs);
        }

        private static string BuildLinkerLibraryArguments(LinkerArguments args)
        {
            var commandArgs = new List<string>();

            // Set options
            // r - Replace existing
            // c - Create without warning if does not exist
            commandArgs.Add("rc");

            // Add the library output file
            var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
            commandArgs.Add($"{ouputPath}{args.Name}.a");

            // Lastly add the file
            commandArgs.AddRange(args.SourceFiles.Select(file => file.Replace(".obj", ".o")));

            return string.Join(" ", commandArgs);
        }

        private static string BuildLinkerExecutableArguments(LinkerArguments args)
        {
            var commandArgs = new List<string>();

            // Add the library output file
            var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
            commandArgs.Add($"-o\"{ouputPath}{args.Name}.exe\"");

            // Add the library files
            commandArgs.AddRange(args.LibraryFiles);

            // Lastly add the file
            commandArgs.AddRange(args.SourceFiles.Select(file => file.Replace(".obj", ".o")));

            return string.Join(" ", commandArgs);
        }

        private static void ProcessLine(string line)
        {
            if (line.Contains("error"))
            {
                Log.Error(line);
            }
            else if (line.Contains("warning"))
            {
                Log.Warning(line);
            }
            else
            {
                Log.Info(line);
            }
        }
    }
}
