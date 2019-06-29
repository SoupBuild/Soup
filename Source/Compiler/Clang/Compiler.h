// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::Clang
{
    /// <summary>
    /// The Clang compiler implementation
    /// </summary>
    export class Compiler : public ICompiler
    {
    private:
        // static Path ToolsPath = "C:/Program Files/llvm/";
        static constexpr std::string_view ToolsPath = "D:/Repos/llvm/build/Release/";
        static constexpr std::string_view CompilerExecutable = "bin/clang++.exe";
        // static Regex IsHeaderIncludeStart = new Regex("^[\\.]+ ", RegexOptions.Compiled);
        // static Regex IsWarningMessage = new Regex("^.* warning: ", RegexOptions.Compiled);
        // static Regex IsErrorMessage = new Regex("^.* error: ", RegexOptions.Compiled);

        // Stack<HeaderInclude> _currentIncludes = new Stack<HeaderInclude>();
        // bool _inWarningMessage = false;
        // bool _inErrorMessage = false;

    public:
        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        virtual const std::string& GetName() const override final
        {
            static std::string value = "Clang";
            return value;
        }

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        virtual const std::string& GetObjectFileExtension() const override final
        {
            static std::string value = "o";
            return value;
        }

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        virtual const std::string& GetModuleFileExtension() const override final
        {
            static std::string value = "pcm";
            return value;
        }

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        virtual const std::string& GetStaticLibraryFileExtension() const override final
        {
            static std::string value = "a";
            return value;
        }

        /// <summary>
        /// Compile
        /// </summary>
        virtual CompileResult Compile(const CompileArguments& args) override final
        {
            //     // Set the working directory to the output directory
            //     var workingDirectory = Path.Combine(args.RootDirectory, args.OutputDirectory);

            auto compilerPath = Path(ToolsPath) + Path(CompilerExecutable);
            auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

            //     Log.Info($"{file}");
            //     Log.Verbose($"PWD={workingDirectory}");
            //     Log.Verbose($"{compiler} {commandArgs}");

            //     // Add a single root element if includes requested
            //     if (args.GenerateIncludeTree)
            //     {
            //         _currentIncludes.Push(new HeaderInclude()
            //         {
            //             Filename = file,
            //         });
            //     }

            //     _inWarningMessage = false;
            //     _inErrorMessage = false;
            //     using (Process process = new Process())
            //     {
            //         process.StartInfo.UseShellExecute = false;
            //         process.StartInfo.RedirectStandardOutput = true;
            //         process.StartInfo.RedirectStandardError = true;
            //         process.StartInfo.FileName = compiler;
            //         process.StartInfo.WorkingDirectory = workingDirectory;
            //         process.StartInfo.Arguments = commandArgs;

            //         process.OutputDataReceived += ProcessOutputDataReceived;
            //         process.ErrorDataReceived += ProcessErrorDataReceived;

            //         process.Start();
            //         process.BeginOutputReadLine();
            //         process.BeginErrorReadLine();
            //         process.WaitForExit();

            auto result = IProcessManager::Current().Execute(compilerPath, commandArgs);

            // if (process.ExitCode != 0)
            // {
            //     throw new InvalidOperationException();
            // }

            //         HeaderInclude result = null;

            //         // Check if requested include headers
            //         if (args.GenerateIncludeTree)
            //         {
            //             // Move up to the root node
            //             while (_currentIncludes.Count > 1)
            //             {
            //                 _currentIncludes.Pop();
            //             }

            //             if (_currentIncludes.Count != 1)
            //             {
            //                 throw new InvalidOperationException("Expected one root includes node.");
            //             }

            //             result = _currentIncludes.Pop();
            //         }

            //         return Task.FromResult(result);
            //     }

            return CompileResult();
        }

        /// <summary>
        /// Link library
        /// </summary>
        virtual void LinkLibrary(const LinkerArguments& args) override final
        {
            // Set the working directory to the output directory
            // var workingDirectory = args.RootDirectory;

            // string linker = Path.Combine(ToolsPath, @"bin\llvm-ar.exe");
            // var linkerArgs = BuildLinkerLibraryArguments(args);

            // Log.Verbose($"PWD={workingDirectory}");
            // Log.Verbose($"{linker} {linkerArgs}");

            // _inWarningMessage = false;
            // _inErrorMessage = false;
            // using (Process process = new Process())
            // {
            //     process.StartInfo.UseShellExecute = false;
            //     process.StartInfo.RedirectStandardOutput = true;
            //     process.StartInfo.RedirectStandardError = true;
            //     process.StartInfo.FileName = linker;
            //     process.StartInfo.WorkingDirectory = workingDirectory;
            //     process.StartInfo.Arguments = linkerArgs;

            //     process.OutputDataReceived += ProcessOutputDataReceived;
            //     process.ErrorDataReceived += ProcessErrorDataReceived;

            //     process.Start();
            //     process.BeginOutputReadLine();
            //     process.BeginErrorReadLine();
            //     process.WaitForExit();

            //     if (process.ExitCode != 0)
            //     {
            //         throw new InvalidOperationException();
            //     }

            //     return Task.CompletedTask;
            // }
        }

        /// <summary>
        /// Link Executable
        /// </summary>
        virtual void LinkExecutable(const LinkerArguments& args) override final
        {
            // // Set the working directory to the output directory
            // var workingDirectory = args.RootDirectory;

            // string linker = Path.Combine(ToolsPath, @"bin\clang++.exe");
            // var linkerArgs = BuildLinkerExecutableArguments(args);

            // Log.Verbose($"PWD={workingDirectory}");
            // Log.Verbose($"{linker} {linkerArgs}");

            // _inWarningMessage = false;
            // _inErrorMessage = false;
            // using (Process process = new Process())
            // {
            //     process.StartInfo.UseShellExecute = false;
            //     process.StartInfo.RedirectStandardOutput = true;
            //     process.StartInfo.RedirectStandardError = true;
            //     process.StartInfo.FileName = linker;
            //     process.StartInfo.WorkingDirectory = workingDirectory;
            //     process.StartInfo.Arguments = linkerArgs;

            //     process.OutputDataReceived += ProcessOutputDataReceived;
            //     process.ErrorDataReceived += ProcessErrorDataReceived;

            //     process.Start();
            //     process.BeginOutputReadLine();
            //     process.BeginErrorReadLine();
            //     process.WaitForExit();

            //     if (process.ExitCode != 0)
            //     {
            //         throw new InvalidOperationException();
            //     }

            //     return Task.CompletedTask;
            // }
        }

    private:
        // private string BuildLinkerLibraryArguments(LinkerArguments args)
        // {
        //     var commandArgs = new List<string>();

        //     // Set options
        //     // r - Replace existing
        //     // c - Create without warning if does not exist
        //     commandArgs.Add("rc");

        //     // Add the library output file
        //     var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
        //     commandArgs.Add($"{ouputPath}{args.Name}.{StaticLibraryFileExtension}");

        //     // Lastly add the file
        //     commandArgs.AddRange(args.SourceFiles);

        //     return string.Join(" ", commandArgs);
        // }

        // private static string BuildLinkerExecutableArguments(LinkerArguments args)
        // {
        //     var commandArgs = new List<string>();

        //     // Enable verbose output
        //     // commandArgs.Add("-v");

        //     // Add the library output file
        //     var ouputPath = args.OutputDirectory.EnsureTrailingSlash().Replace(@"\", @"\\");
        //     commandArgs.Add($"-o\"{ouputPath}{args.Name}.exe\"");

        //     // Add the library files
        //     commandArgs.AddRange(args.LibraryFiles);

        //     // Lastly add the file
        //     commandArgs.AddRange(args.SourceFiles.Select(file => file.Replace(".obj", ".o")));

        //     return string.Join(" ", commandArgs);
        // }

        // private void ProcessOutputDataReceived(object sender, DataReceivedEventArgs e)
        // {
        //     if (e.Data != null)
        //     {
        //         Log.Info(e.Data);
        //     }
        // }

        // private void ProcessErrorDataReceived(object sender, DataReceivedEventArgs e)
        // {
        //     if (e.Data != null)
        //     {
        //         if (IsWarningMessage.IsMatch(e.Data))
        //         {
        //             _inWarningMessage = true;
        //             _inErrorMessage = false;
        //         }
        //         else if (IsErrorMessage.IsMatch(e.Data))
        //         {
        //             _inWarningMessage = false;
        //             _inErrorMessage = true;
        //         }
        //         else
        //         {
        //             _inWarningMessage = false;
        //             _inErrorMessage = false;
        //         }

        //         if (_inWarningMessage)
        //         {
        //             Log.Warning(e.Data);
        //         }
        //         else if (_inErrorMessage)
        //         {
        //             Log.Error(e.Data);
        //         }
        //         else if (IsHeaderIncludeStart.IsMatch(e.Data))
        //         {
        //             ProcessHeaderInclude(e.Data);
        //         }
        //         else
        //         {
        //             Log.Info(e.Data);
        //         }
        //     }
        // }

        // private void ProcessHeaderInclude(string value)
        // {
        //     // Count depth
        //     int depth = 0;
        //     while (value[depth] == '.')
        //     {
        //         depth++;
        //     }

        //     var filename = value.Substring(depth + 1).Replace("\\\\", "/");
        //     if (!Path.IsPathRooted(filename))
        //         filename = filename.Substring(9); // HACK TODO : Get correct path

        //     var include = new HeaderInclude()
        //     {
        //         Filename = filename,
        //     };

        //     // Move to matching depth
        //     while (_currentIncludes.Count > depth)
        //     {
        //         _currentIncludes.Pop();
        //     }

        //     // Ensure there is a current node
        //     if (depth == 0)
        //     {
        //         throw new InvalidOperationException("Missing root include node.");
        //     }

        //     // Skip precompiled module files
        //     var extension = Path.GetExtension(filename);
        //     if (extension != $".{ModuleFileExtension}")
        //     {
        //         // Add to current
        //         _currentIncludes.Peek().Includes.Add(include);
        //     }

        //     // Set the new include as the current depth
        //     _currentIncludes.Push(include);
        // }
    };
}
