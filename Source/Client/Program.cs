// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System;
    using System.Runtime.InteropServices;
    using System.Threading.Tasks;
    using CommandLine;
    using Compiler;
    using Soup.Api;

    /// <summary>
    /// The root of all evil
    /// </summary>
    public class Program
    {
        /// <summary>
        /// The root of all evil - async style
        /// </summary>
        private static async Task<int> Main(string[] args)
        {
            try
            {
                var arguments = Parser.Default.ParseArguments<
                    BuildOptions,
                    RunOptions,
                    InitializeOptions,
                    InstallOptions,
                    PackOptions,
                    PublishOptions,
                    VersionOptions,
                    ViewOptions>(args);

                // Check shared options
                var initialized = arguments.MapResult(
                        (SharedOptions options) => Setup(options),
                        errors => false);
                if (!initialized)
                {
                    return -1;
                }

                // Map the indivual commands
                await arguments.MapResult(
                        (BuildOptions options) => new BuildCommand(GetUserConfig(), GetCompiler()).InvokeAsync(options),
                        (RunOptions options) => new RunCommand(GetUserConfig(), GetCompiler()).InvokeAsync(options),
                        (InitializeOptions options) => new InitializeCommand().InvokeAsync(options),
                        (InstallOptions options) => new InstallCommand(GetUserConfig(), GetSoupApi()).InvokeAsync(options),
                        (PackOptions options) => new PackCommand().InvokeAsync(options),
                        (PublishOptions options) => new PublishCommand(GetSoupIdentity(), GetSoupApi()).InvokeAsync(options),
                        (VersionOptions options) => new VersionCommand().InvokeAsync(options),
                        (ViewOptions options) => new ViewCommand(GetSoupApi()).InvokeAsync(options),
                        errors => Task.CompletedTask);

                return 0;
            }
            catch (Exception ex)
            {
                Log.Error(ex.ToString());
                return -1;
            }
        }

        private static bool Setup(SharedOptions options)
        {
            Log.EnableVerbose = options.EnableVerbose;

            return true;
        }

        /// <summary>
        /// Load the local user config
        /// </summary>
        private static LocalUserConfig GetUserConfig()
        {
            return new LocalUserConfig();
        }

        /// <summary>
        /// Create the soup api helper
        /// </summary>
        private static ISoupIdentity GetSoupIdentity()
        {
            return new SoupIdentity();
        }

        /// <summary>
        /// Create the soup api helper
        /// </summary>
        private static ISoupApi GetSoupApi()
        {
            return new SoupApi();
        }

        /// <summary>
        /// Setup the required Compiler
        /// </summary>
        private static ICompiler GetCompiler()
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
            {
                // return new Compiler.MSVC.Compiler();
                return new Compiler.Clang.Compiler();
            }
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
            {
                return new Compiler.Clang.Compiler();
            }
            else
            {
                throw new NotSupportedException("Unknown platform.");
            }
        }
    }
}
