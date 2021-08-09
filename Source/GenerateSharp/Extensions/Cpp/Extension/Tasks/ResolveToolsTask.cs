// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Text;

namespace Soup.Build.Cpp
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

			if (!LifetimeManager.Has<IProcessManager>())
			{
				LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();
			}
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

			// Find the MSVC SDK
			var msvcSDKProperties = GetSDKProperties("MSVC", parameters);

			// Use the default version
			var visualCompilerVersion = msvcSDKProperties["Version"].AsString();
			_buildState.LogTrace(TraceLevel.Information, "Using VC Version: " + visualCompilerVersion);

			// Get the final VC tools folder
			var visualCompilerVersionFolder = new Path(msvcSDKProperties["VCToolsRoot"].AsString());

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
			state["MSVC.Version"] = new Value(visualCompilerVersion);
			state["MSVC.VCToolsRoot"] = new Value(visualCompilerVersionFolder.ToString());
			state["MSVC.VCToolsBinaryRoot"] = new Value(vcToolsBinaryFolder.ToString());
			state["MSVC.LinkToolPath"] = new Value(linkToolPath.ToString());
			state["MSVC.LibToolPath"] = new Value(libToolPath.ToString());

			// Allow custom overrides for the compiler path
			if (!state.ContainsKey("MSVC.ClToolPath"))
				state["MSVC.ClToolPath"] = new Value(clToolPath.ToString());

			// Load the Windows sdk
			var windowsSDKProperties = GetSDKProperties("Windows", parameters);

			// Calculate the windows kits directory
			var windows10KitPath = new Path(windowsSDKProperties["RootPath"].AsString());
			var windows10KitIncludePath = windows10KitPath + new Path("/include/");
			var windows10KitLibPath = windows10KitPath + new Path("/Lib/");

			var windowsKitVersion = windowsSDKProperties["Version"].AsString();

			_buildState.LogTrace(TraceLevel.Information, "Using Windows Kit Version: " + windowsKitVersion);
			var windows10KitVersionIncludePath = windows10KitIncludePath + new Path(windowsKitVersion + "/");
			var windows10KitVersionLibPath = windows10KitLibPath + new Path(windowsKitVersion + "/");

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

		private IValueTable GetSDKProperties(string name, IValueTable state)
		{
			foreach (var sdk in state["SDKs"].AsList())
			{
				var sdkTable = sdk.AsTable();
				if (sdkTable.TryGetValue("Name", out var nameValue))
				{
					if (nameValue.AsString() == name)
					{
						return sdkTable["Properties"].AsTable();
					}
				}
			}

			throw new InvalidOperationException($"Missing SDK {name}");
		}
	}
}
