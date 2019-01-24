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
    using System.Text.RegularExpressions;
    using System.Threading.Tasks;

    /// <summary>
    /// The MSVC compiler implementation
    /// </summary>
    public class Compiler : ICompiler
    {
        //private static string ToolsPath => @"C:\Program Files\llvm\";
        private static string ToolsPath => @"D:\Repos\llvm\build\Release";
        private static Regex IsWarningMessage = new Regex("^.* warning: ", RegexOptions.Compiled);
        private static Regex IsErrorMessage = new Regex("^.* error: ", RegexOptions.Compiled);

        private bool _inWarningMessage = false;
        private bool _inErrorMessage = false;

        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        public string Name => "Clang";

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        public string ObjectFileExtension => "o";

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        public string ModuleFileExtension => "pcm";

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        public string StaticLibraryFileExtension => "a";

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

            _inWarningMessage = false;
            _inErrorMessage = false;
            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.FileName = compiler;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = commandArgs;

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

            _inWarningMessage = false;
            _inErrorMessage = false;
            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.FileName = linker;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = linkerArgs;

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

            _inWarningMessage = false;
            _inErrorMessage = false;
            using (Process process = new Process())
            {
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.FileName = linker;
                process.StartInfo.WorkingDirectory = workingDirectory;
                process.StartInfo.Arguments = linkerArgs;

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

                return Task.CompletedTask;
            }
        }

        private string BuildCompilerArguments(CompilerArguments args, string workingDirectory)
        {
            // Calculate object output file
            var rootPath = Path.GetRelativePath(workingDirectory, args.RootDirectory);

            var commandArgs = new List<string>();

            // Disable ms compatibility (workaround for bug with inplicit types in pcm)
            // commandArgs.Add("-fno-ms-compatibility");

            // Allow public std visible during link time
            commandArgs.AddRange(new string[] { "-Xclang", "-flto-visibility-public-std" });

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
                commandArgs.Add($"-I\"{Path.Combine(rootPath, directory)}\"");
            }

            // Set the preprocessor definitions
            foreach (var definition in args.PreprocessorDefinitions)
            {
                commandArgs.Add($"-D{definition}");
            }

            // Ignore Standard Include Paths to prevent pulling in accidental headers
            // commandArgs.Add("-X");

            // Add in the std include paths

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
                commandArgs.Add($"-fmodule-file=\"{Path.Combine(rootPath, module)}\"");
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
                var outputFile = $"{Path.GetFileNameWithoutExtension(sourceFile)}.{ModuleFileExtension}";
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

        private string BuildLinkerLibraryArguments(LinkerArguments args)
        {
            var commandArgs = new List<string>();

            // Set options
            // r - Replace existing
            // c - Create without warning if does not exist
            commandArgs.Add("rc");

            // Add the library output file
            var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
            commandArgs.Add($"{ouputPath}{args.Name}.{StaticLibraryFileExtension}");

            // Lastly add the file
            commandArgs.AddRange(args.SourceFiles);

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

        private void ProcessOutputDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                Log.Info(e.Data);
            }
        }

        private void ProcessErrorDataReceived(object sender, DataReceivedEventArgs e)
        {
            if (e.Data != null)
            {
                if (IsWarningMessage.IsMatch(e.Data))
                {
                    _inWarningMessage = true;
                    _inErrorMessage = false;
                }
                else if (IsErrorMessage.IsMatch(e.Data))
                {
                    _inWarningMessage = false;
                    _inErrorMessage = true;
                }

                if (_inWarningMessage)
                {
                    Log.Warning(e.Data);
                }
                else if (_inErrorMessage)
                {
                    Log.Error(e.Data);
                }
                else
                {
                    Log.Info(e.Data);
                }
            }
        }
    }
}
