// <copyright company="Soup" file="Program.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System;
	using System.Collections.Generic;
	using System.IO;
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
		/// The collection of all known commands for the command line application
		/// </summary>
		private static IReadOnlyList<ICommand> _commands = new List<ICommand>()
		{

		};

		/// <summary>
		/// The root of all evil - async style
		/// </summary>
		private static async Task Main(string[] args)
		{
			Parser.Default.ParseArguments<
				BuildOptions,
				InitializeOptions,
				InstallOptions,
				PackOptions,
				PublishOptions,
				VersionOptions,
				ViewOptions>(args)
				.MapResult(
					(BuildOptions options) => new BuildCommand().InvokeAsync(options),
					(InitializeOptions options) => new InitializeCommand().InvokeAsync(options),
					(InstallOptions options) => new InstallCommand().InvokeAsync(options),
					(PackOptions options) => new PackCommand().InvokeAsync(options),
					(PublishOptions options) => new PublishCommand().InvokeAsync(options),
					(VersionOptions options) => new VersionCommand().InvokeAsync(options),
					(ViewOptions options) => new ViewCommand().InvokeAsync(options),
					errors => Task.CompletedTask).Wait();

			// Load the user configuration settings
			var userConfig = new LocalUserConfig();
			var stagingDirectory = Path.Combine(userConfig.PackageStore, Constants.StagingFolderName);

			// Ensure we are in a clean state
			if (Directory.Exists(stagingDirectory))
			{
				Log.Warning("The staging directory was not cleaned up!");
				Directory.Delete(stagingDirectory, true);
			}
		}

		/// <summary>
		/// Setup the required Compiler
		/// </summary>
		private static ICompiler GetCompiler()
		{
			if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
			{
				return new Compiler.MSVC.Compiler();
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
