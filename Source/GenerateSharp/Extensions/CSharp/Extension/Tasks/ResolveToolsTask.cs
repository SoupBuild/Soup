// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;

namespace Soup.Build.CSharp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class ResolveToolsTask : IBuildTask
	{
		private IBuildState _buildState;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
		};

		public ResolveToolsTask(IBuildState buildState)
		{
			_buildState = buildState;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var state = _buildState.ActiveState;
			var parameters = state["Parameters"].AsTable();

			var systemName = parameters["System"].AsString();
			var architectureName = parameters["Architecture"].AsString();

			if (systemName != "win32")
				throw new InvalidOperationException("Win32 is the only supported system... so far.");

			// Check if skip platform includes was specified
			bool skipPlatform = false;
			if (state.TryGetValue("SkipPlatform", out var skipPlatformValue))
			{
				skipPlatform = skipPlatformValue.AsBoolean();
			}

			// Find the location of the Windows SDK
			var visualStudioInstallRoot = FindVSInstallRoot();
			_buildState.LogTrace(TraceLevel.Information, "Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Use the default version
			var visualCompilerVersion = FindDefaultVCToolsVersion(visualStudioInstallRoot);
			_buildState.LogTrace(TraceLevel.Information, "Using VC Version: " + visualCompilerVersion);

			// Calculate the final VC tools folder
			var visualCompilerVersionFolder =
				visualStudioInstallRoot + new Path("/VC/Tools/MSVC/") + new Path(visualCompilerVersion);

			// Sey the VC tools binary folder
			Path vcToolsBinaryFolder;
			if (architectureName == "x64")
				vcToolsBinaryFolder = visualCompilerVersionFolder + new Path("/bin/Hostx64/x64/");
			else if (architectureName == "x86")
				vcToolsBinaryFolder = visualCompilerVersionFolder + new Path("/bin/Hostx64/x86/");
			else
				throw new InvalidOperationException("Unknown architecture.");

			var clToolPath = vcToolsBinaryFolder + new Path("cl.exe");
			var linkToolPath = vcToolsBinaryFolder + new Path("link.exe");
			var libToolPath = vcToolsBinaryFolder + new Path("lib.exe");

			// Save the build properties
			state["MSVS.InstallRoot"] = new Value(visualStudioInstallRoot.ToString());
			state["MSVC.Version"] = new Value(visualCompilerVersion);
			state["MSVC.VCToolsRoot"] = new Value(visualCompilerVersionFolder.ToString());
			state["MSVC.VCToolsBinaryRoot"] = new Value(vcToolsBinaryFolder.ToString());
			state["MSVC.LinkToolPath"] = new Value(linkToolPath.ToString());
			state["MSVC.LibToolPath"] = new Value(libToolPath.ToString());

			// Allow custom overrides for the compiler path
			if (!state.ContainsKey("MSVC.ClToolPath"))
				state["MSVC.ClToolPath"] = new Value(clToolPath.ToString());

			// Calculate the windows kits directory
			var windows10KitPath = new Path("C:/Program Files (x86)/Windows Kits/10/");
			var windows10KitIncludePath = windows10KitPath + new Path("/include/");
			var windows10KitLibPath = windows10KitPath + new Path("/Lib/");

			var windowsKitVersion = FindNewestWindows10KitVersion(windows10KitIncludePath);

			_buildState.LogTrace(TraceLevel.Information, "Using Windows Kit Version: " + windowsKitVersion);
			var windows10KitVersionIncludePath = windows10KitIncludePath + new Path(windowsKitVersion);
			var windows10KitVersionLibPath = windows10KitLibPath + new Path(windowsKitVersion);

			// Set the include paths
			var platformIncludePaths = new List<Path>();
			if (!skipPlatform)
			{
				platformIncludePaths = new List<Path>()
				{
					visualCompilerVersionFolder + new Path("/include/"),
					windows10KitVersionIncludePath + new Path("/ucrt/"),
					windows10KitVersionIncludePath + new Path("/um/"),
					windows10KitVersionIncludePath + new Path("/shared/"),
				};
			}

			// Set the include paths
			var platformLibraryPaths = new List<Path>();
			if (!skipPlatform)
			{
				if (architectureName == "x64")
				{
					platformLibraryPaths.Add(windows10KitVersionLibPath + new Path("/ucrt/x64/"));
					platformLibraryPaths.Add(windows10KitVersionLibPath + new Path("/um/x64/"));
					platformLibraryPaths.Add(visualCompilerVersionFolder + new Path("/atlmfc/lib/x64/"));
					platformLibraryPaths.Add(visualCompilerVersionFolder + new Path("/lib/x64/"));
				}
				else if (architectureName == "x86")
				{
					platformLibraryPaths.Add(windows10KitVersionLibPath + new Path("/ucrt/x86/"));
					platformLibraryPaths.Add(windows10KitVersionLibPath + new Path("/um/x86/"));
					platformLibraryPaths.Add(visualCompilerVersionFolder + new Path("/atlmfc/lib/x86/"));
					platformLibraryPaths.Add(visualCompilerVersionFolder + new Path("/lib/x86/"));
				}
			}

			// Set the platform definitions
			var platformPreprocessorDefinitions = new List<string>()
			{
				// "_DLL", // Link against the dynamic runtime dll
				// "_MT", // Use multithreaded runtime
			};

			if (architectureName == "x86")
				platformPreprocessorDefinitions.Add("WIN32");

			// Set the platform libraries
			var platformLibraries = new List<Path>()
			{
				new Path("kernel32.lib"),
				new Path("user32.lib"),
				new Path("gdi32.lib"),
				new Path("winspool.lib"),
				new Path("comdlg32.lib"),
				new Path("advapi32.lib"),
				new Path("shell32.lib"),
				new Path("ole32.lib"),
				new Path("oleaut32.lib"),
				new Path("uuid.lib"),
				// Path("odbc32.lib"),
				// Path("odbccp32.lib"),
				// Path("crypt32.lib"),
			};

			// if (_options.Configuration == "debug")
			// {
			// 	// arguments.PlatformPreprocessorDefinitions.push_back("_DEBUG");
			// 	arguments.PlatformLibraries = std::vector<Path>({
			// 		Path("msvcprtd.lib"),
			// 		Path("msvcrtd.lib"),
			// 		Path("ucrtd.lib"),
			// 		Path("vcruntimed.lib"),
			// 	});
			// }
			// else
			// {
			// 	arguments.PlatformLibraries = std::vector<Path>({
			// 		Path("msvcprt.lib"),
			// 		Path("msvcrt.lib"),
			// 		Path("ucrt.lib"),
			// 		Path("vcruntime.lib"),
			// 	});
			// }
	
			state.EnsureValueList("PlatformIncludePaths").SetAll(platformIncludePaths);
			state.EnsureValueList("PlatformLibraryPaths").SetAll(platformLibraryPaths);
			state.EnsureValueList("PlatformLibraries").SetAll(platformLibraries);
			state.EnsureValueList("PlatformPreprocessorDefinitions").SetAll(platformPreprocessorDefinitions);
		}

		private Path FindVSInstallRoot()
		{
			// Find a copy of visual studio that has the required VisualCompiler
			var executablePath = new Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			var workingDirectory = new Path("./");
			var argumentList = new List<string>()
			{
				"-latest",
				"-products",
				"*",
				"-requires",
				"Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
				"-property",
				"installationPath",
			};

			// Check if we should include pre-release versions
			bool includePrerelease = true;
			if (includePrerelease)
			{
				argumentList.Add("-prerelease");
			}

			// Execute the requested target
			var arguments = CombineArguments(argumentList);
			_buildState.LogTrace(TraceLevel.Debug, executablePath.ToString() + " " + arguments);
			var processInfo = new ProcessStartInfo()
			{
				FileName = executablePath.ToString(),
				Arguments = arguments,
				WorkingDirectory = workingDirectory.ToString(),
				RedirectStandardOutput = true,
				RedirectStandardError = true,
			};
			var process = Process.Start(processInfo);
			if (process is null)
				throw new InvalidOperationException("Failed to start process");

			process.WaitForExit();

			var stdOut = process.StandardOutput;
			var stdErr = process.StandardError.ReadToEnd();
			var exitCode = process.ExitCode;

			if (!string.IsNullOrEmpty(stdErr))
			{
				_buildState.LogTrace(TraceLevel.Error, stdErr);
				throw new InvalidOperationException("VSWhere failed.");
			}

			if (exitCode != 0)
			{
				// TODO: Return error code
				_buildState.LogTrace(TraceLevel.Error, "FAILED");
				throw new InvalidOperationException("VSWhere failed.");
			}

			// The first line is the path
			var path = stdOut.ReadLine();
			if (path is null)
			{
				_buildState.LogTrace(TraceLevel.Error, "Failed to parse vswhere output.");
				throw new InvalidOperationException("Failed to parse vswhere output.");
			}

			return new Path(path);
		}

		private string FindDefaultVCToolsVersion(
			Path visualStudioInstallRoot)
		{
			// Check the default tools version
			var visualCompilerToolsDefaultVersionFile =
				visualStudioInstallRoot + new Path("VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt");
			if (!System.IO.File.Exists(visualCompilerToolsDefaultVersionFile.ToString()))
			{
				_buildState.LogTrace(TraceLevel.Error, "VisualCompilerToolsDefaultVersionFile file does not exist: " + visualCompilerToolsDefaultVersionFile.ToString());
				throw new InvalidOperationException("VisualCompilerToolsDefaultVersionFile file does not exist.");
			}

			// Read the entire file into a string
			using (var file = System.IO.File.OpenRead(visualCompilerToolsDefaultVersionFile.ToString()))
			using (var reader = new System.IO.StreamReader(file))
			{
				// The first line is the version
				var version = reader.ReadLine();
				if (version is null)
				{
					_buildState.LogTrace(TraceLevel.Error, "Failed to parse version from file.");
					throw new InvalidOperationException("Failed to parse version from file.");
				}

				return version;
			}
		}

		private string FindNewestWindows10KitVersion(
			Path windows10KitIncludePath)
		{
			// Check the default tools version
			_buildState.LogTrace(TraceLevel.Debug, "FindNewestWindows10KitVersion: " + windows10KitIncludePath.ToString());
			var currentVersion = new SemanticVersion(0, 0, 0);
			foreach (var child in System.IO.Directory.EnumerateDirectories(windows10KitIncludePath.ToString()))
			{
				var name = new Path(child).GetFileName();
				_buildState.LogTrace(TraceLevel.Debug, "CheckFile: " + name);
				var platformVersion = name.Substring(0, 3);
				if (platformVersion == "10.")
				{
					// Parse the version string
					var version = SemanticVersion.Parse(name.Substring(3));
					if (version > currentVersion)
						currentVersion = version;
				}
				else
				{
					_buildState.LogTrace(TraceLevel.Warning, "Unexpected Kit Version: " + name);
				}
			}

			if (currentVersion == new SemanticVersion(0, 0, 0))
				throw new InvalidOperationException("Could not find a minimum Windows 10 Kit Version");

			// The first line is the version
			var result = "10." + currentVersion.ToString();
			return result;
		}

		private static string CombineArguments(IList<string> args)
		{
			var argumentString = new StringBuilder();
			bool isFirst = true;
			foreach (var arg in args)
			{
				if (!isFirst)
					argumentString.Append(" ");

				argumentString.Append(arg);
				isFirst = false;
			}

			return argumentString.ToString();
		}
	}
}
