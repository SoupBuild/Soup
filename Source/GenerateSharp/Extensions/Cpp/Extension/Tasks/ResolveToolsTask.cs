// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Cpp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class ResolveToolsTask : IBuildTask
	{
		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var state = buildState.GetActiveState();
			var parameters = state.GetValue("Parameters").AsTable();

			var systemName = string(parameters.GetValue("System").AsString().GetValue());
			var architectureName = string(parameters.GetValue("Architecture").AsString().GetValue());

			if (systemName != "win32")
				throw std::runtime_error("Win32 is the only supported system... so far.");

			// Check if skip platform includes was specified
			bool skipPlatform = false;
			if (state.HasValue("SkipPlatform"))
			{
				skipPlatform = state.GetValue("SkipPlatform").AsBoolean().GetValue();
			}

			// Find the location of the Windows SDK
			var visualStudioInstallRoot = FindVSInstallRoot(buildState);
			buildState.LogInfo("Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Use the default version
			var visualCompilerVersion = FindDefaultVCToolsVersion(buildState, visualStudioInstallRoot);
			buildState.LogInfo("Using VC Version: " + visualCompilerVersion);

			// Calculate the final VC tools folder
			var visualCompilerVersionFolder =
				visualStudioInstallRoot + Path("/VC/Tools/MSVC/") + Path(visualCompilerVersion);

			// Sey the VC tools binary folder
			Path vcToolsBinaryFolder;
			if (architectureName == "x64")
				vcToolsBinaryFolder = visualCompilerVersionFolder + Path("/bin/Hostx64/x64/");
			else if (architectureName == "x86")
				vcToolsBinaryFolder = visualCompilerVersionFolder + Path("/bin/Hostx64/x86/");
			else
				throw std::runtime_error("Unknown architecture.");

			var clToolPath = vcToolsBinaryFolder + Path("cl.exe");
			var linkToolPath = vcToolsBinaryFolder + Path("link.exe");
			var libToolPath = vcToolsBinaryFolder + Path("lib.exe");

			// Save the build properties
			state.EnsureValue("MSVS.InstallRoot").SetValueString(visualStudioInstallRoot.ToString());
			state.EnsureValue("MSVC.Version").SetValueString(visualCompilerVersion);
			state.EnsureValue("MSVC.VCToolsRoot").SetValueString(visualCompilerVersionFolder.ToString());
			state.EnsureValue("MSVC.VCToolsBinaryRoot").SetValueString(vcToolsBinaryFolder.ToString());
			state.EnsureValue("MSVC.LinkToolPath").SetValueString(linkToolPath.ToString());
			state.EnsureValue("MSVC.LibToolPath").SetValueString(libToolPath.ToString());

			// Allow custom overrides for the compiler path
			if (!state.HasValue("MSVC.ClToolPath"))
				state.EnsureValue("MSVC.ClToolPath").SetValueString(clToolPath.ToString());

			// Calculate the windows kits directory
			var windows10KitPath = Path("C:/Program Files (x86)/Windows Kits/10/");
			var windows10KitIncludePath = windows10KitPath + Path("/include/");
			var windows10KitLibPath = windows10KitPath + Path("/Lib/");

			var windowsKitVersion = FindNewestWindows10KitVersion(buildState, windows10KitIncludePath);

			buildState.LogInfo("Using Windows Kit Version: " + windowsKitVersion);
			var windows10KitVersionIncludePath = windows10KitIncludePath + Path(windowsKitVersion);
			var windows10KitVersionLibPath = windows10KitLibPath + Path(windowsKitVersion);

			// Set the include paths
			var platformIncludePaths = std::vector<Path>();
			if (!skipPlatform)
			{
				platformIncludePaths = std::vector<Path>({
					visualCompilerVersionFolder + Path("/include/"),
					windows10KitVersionIncludePath + Path("/ucrt/"),
					windows10KitVersionIncludePath + Path("/um/"),
					windows10KitVersionIncludePath + Path("/shared/"),
				});
			}

			// Set the include paths
			var platformLibraryPaths = std::vector<Path>();
			if (!skipPlatform)
			{
				if (architectureName == "x64")
				{
					platformLibraryPaths.push_back(windows10KitVersionLibPath + Path("/ucrt/x64/"));
					platformLibraryPaths.push_back(windows10KitVersionLibPath + Path("/um/x64/"));
					platformLibraryPaths.push_back(visualCompilerVersionFolder + Path("/atlmfc/lib/x64/"));
					platformLibraryPaths.push_back(visualCompilerVersionFolder + Path("/lib/x64/"));
				}
				else if (architectureName == "x86")
				{
					platformLibraryPaths.push_back(windows10KitVersionLibPath + Path("/ucrt/x86/"));
					platformLibraryPaths.push_back(windows10KitVersionLibPath + Path("/um/x86/"));
					platformLibraryPaths.push_back(visualCompilerVersionFolder + Path("/atlmfc/lib/x86/"));
					platformLibraryPaths.push_back(visualCompilerVersionFolder + Path("/lib/x86/"));
				}
			}

			// Set the platform definitions
			var platformPreprocessorDefinitions = std::vector<string>({
				// "_DLL", // Link against the dynamic runtime dll
				// "_MT", // Use multithreaded runtime
			});

			if (architectureName == "x86")
				platformPreprocessorDefinitions.push_back("WIN32");

			// Set the platform libraries
			var platformLibraries = std::vector<Path>({
				Path("kernel32.lib"),
				Path("user32.lib"),
				Path("gdi32.lib"),
				Path("winspool.lib"),
				Path("comdlg32.lib"),
				Path("advapi32.lib"),
				Path("shell32.lib"),
				Path("ole32.lib"),
				Path("oleaut32.lib"),
				Path("uuid.lib"),
				// Path("odbc32.lib"),
				// Path("odbccp32.lib"),
				// Path("crypt32.lib"),
			});

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
	
			state.EnsureValue("PlatformIncludePaths").SetValuePathList(platformIncludePaths);
			state.EnsureValue("PlatformLibraryPaths").SetValuePathList(platformLibraryPaths);
			state.EnsureValue("PlatformLibraries").SetValuePathList(platformLibraries);
			state.EnsureValue("PlatformPreprocessorDefinitions").SetValueStringList(platformPreprocessorDefinitions);
		}

	private:
		Path FindVSInstallRoot(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			// Find a copy of visual studio that has the required VisualCompiler
			var executablePath = Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			var workingDirectory = Path("./");
			var argumentList = std::vector<string>({
				"-latest",
				"-products",
				"*",
				"-requires",
				"Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
				"-property",
				"installationPath",
			});

			// Check if we should include pre-release versions
			bool includePrerelease = true;
			if (includePrerelease)
			{
				argumentList.push_back("-prerelease");
			}

			// Execute the requested target
			var arguments = CombineArguments(argumentList);
			buildState.LogDebug(executablePath.ToString() + " " + arguments);
			var process = System::IProcessManager::Current().CreateProcess(
				executablePath,
				arguments,
				workingDirectory);
			process->Start();
			process->WaitForExit();

			var stdOut = process->GetStandardOutput();
			var stdErr = process->GetStandardError();
			var exitCode = process->GetExitCode();

			if (!stdErr.empty())
			{
				buildState.LogError(stdErr);
				throw std::runtime_error("VSWhere failed.");
			}

			if (exitCode != 0)
			{
				// TODO: Return error code
				buildState.LogError("FAILED");
				throw std::runtime_error("VSWhere failed.");
			}

			var stream = std::istringstream(stdOut);

			// The first line is the path
			var path = string();
			if (!std::getline(stream, path, '\r'))
			{
				buildState.LogError("Failed to parse vswhere output.");
				throw std::runtime_error("Failed to parse vswhere output.");
			}

			return Path(path);
		}

		string FindDefaultVCToolsVersion(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const Path& visualStudioInstallRoot)
		{
			// Check the default tools version
			var visualCompilerToolsDefaultVersionFile =
				visualStudioInstallRoot + Path("VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt");
			if (!System::IFileSystem::Current().Exists(visualCompilerToolsDefaultVersionFile))
			{
				buildState.LogError("VisualCompilerToolsDefaultVersionFile file does not exist: " + visualCompilerToolsDefaultVersionFile.ToString());
				throw std::runtime_error("VisualCompilerToolsDefaultVersionFile file does not exist.");
			}

			// Read the entire file into a string
			var file = System::IFileSystem::Current().OpenRead(visualCompilerToolsDefaultVersionFile, false);

			// The first line is the version
			var version = string();
			if (!std::getline(file->GetInStream(), version, '\n'))
			{
				buildState.LogError("Failed to parse version from file.");
				throw std::runtime_error("Failed to parse version from file.");
			}

			return version;
		}

		string FindNewestWindows10KitVersion(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const Path& windows10KitIncludePath)
		{
			// Check the default tools version
			buildState.LogDebug("FindNewestWindows10KitVersion: " + windows10KitIncludePath.ToString());
			var currentVersion = SemanticVersion(0, 0, 0);
			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(windows10KitIncludePath))
			{
				var name = child.Path.GetFileName();
				buildState.LogDebug("CheckFile: " + string(name));
				var platformVersion = name.substr(0, 3);
				if (platformVersion == "10.")
				{
					// Parse the version string
					var version = SemanticVersion::Parse(name.substr(3));
					if (version > currentVersion)
						currentVersion = version;
				}
				else
				{
					buildState.LogWarning("Unexpected Kit Version: " + string(name));
				}
			}

			if (currentVersion == SemanticVersion(0, 0, 0))
				throw std::runtime_error("Could not find a minimum Windows 10 Kit Version");

			// The first line is the version
			var version = "10." + currentVersion.ToString();
			return version;
		}

		static string CombineArguments(const std::vector<string>& args)
		{
			var argumentString = stringstream();
			bool isFirst = true;
			for (auto& arg : args)
			{
				if (!isFirst)
					argumentString << " ";

				argumentString << arg;
				isFirst = false;
			}

			return argumentString.str();
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
