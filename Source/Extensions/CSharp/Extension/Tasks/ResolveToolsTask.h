// <copyright file="ResolveToolsTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class ResolveToolsTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "ResolveTools";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				Execute(buildStateWrapper);
				return Soup::Build::ApiCallResult::Success;
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Soup::Build::ApiCallResult::Error;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto state = buildState.GetActiveState();

			auto systemName = std::string(state.GetValue("BuildSystem").AsString().GetValue());
			auto architectureName = std::string(state.GetValue("BuildArchitecture").AsString().GetValue());

			if (systemName != "win32")
				throw std::runtime_error("Win32 is the only supported system... so far.");

			// Find the location of the Windows SDK
			auto visualStudioInstallRoot = FindVSInstallRoot(buildState);
			buildState.LogInfo("Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Use the default version
			auto visualCompilerVersion = FindDefaultVCToolsVersion(buildState, visualStudioInstallRoot);
			buildState.LogInfo("Using VC Version: " + visualCompilerVersion);

			// Calculate the final VC tools folder
			auto visualCompilerVersionFolder =
				visualStudioInstallRoot + Path("/VC/Tools/MSVC/") + Path(visualCompilerVersion);

			// Sey the VC tools binary folder
			Path vcToolsBinaryFolder;
			if (architectureName == "x64")
				vcToolsBinaryFolder = visualCompilerVersionFolder + Path("/bin/Hostx64/x64/");
			else if (architectureName == "x86")
				vcToolsBinaryFolder = visualCompilerVersionFolder + Path("/bin/Hostx64/x86/");
			else
				throw std::runtime_error("Unknown architecture.");

			// Save the build properties
			state.EnsureValue("MSVS.InstallRoot").SetValueString(visualStudioInstallRoot.ToString());
			state.EnsureValue("MSVC.Version").SetValueString(visualCompilerVersion);
			state.EnsureValue("MSVC.VCToolsRoot").SetValueString(visualCompilerVersionFolder.ToString());
			state.EnsureValue("MSVC.VCToolsBinaryRoot").SetValueString(vcToolsBinaryFolder.ToString());

			// Calculate the windows kits directory
			auto windows10KitPath = Path("C:/Program Files (x86)/Windows Kits/10/");
			auto windows10KitIncludePath = windows10KitPath + Path("/include/");
			auto windows10KitLibPath = windows10KitPath + Path("/Lib/");

			auto windowsKitVersion = FindNewestWindows10KitVersion(buildState, windows10KitIncludePath);

			buildState.LogInfo("Using Windows Kit Version: " + windowsKitVersion);
			auto windows10KitVersionIncludePath = windows10KitIncludePath + Path(windowsKitVersion);
			auto windows10KitVersionLibPath = windows10KitLibPath + Path(windowsKitVersion);

			// Set the include paths
			auto platformIncludePaths = std::vector<Path>({
				visualCompilerVersionFolder + Path("/include/"),
				windows10KitVersionIncludePath + Path("/ucrt/"),
				windows10KitVersionIncludePath + Path("/um/"),
				windows10KitVersionIncludePath + Path("/shared/"),
			});

			// Set the platform definitions
			auto platformPreprocessorDefinitions = std::vector<std::string>({
				"TRACE",
				"DEBUG",
				"NETCOREAPP",
				"NETCOREAPP3_1",
			});

			// Set the platform libraries
			// TODO: "C:\Program Files\dotnet\packs\Microsoft.NETCore.App.Ref\3.1.0\ref\netcoreapp3.1\"
			auto platformLibraries = std::vector<Path>({
				Path("Microsoft.CSharp.dll"),
				Path("Microsoft.VisualBasic.Core.dll"),
				Path("Microsoft.VisualBasic.dll"),
				Path("Microsoft.Win32.Primitives.dll"),
				Path("mscorlib.dll"),
				Path("netstandard.dll"),
				Path("System.AppContext.dll"),
				Path("System.Buffers.dll"),
				Path("System.Collections.Concurrent.dll"),
				Path("System.Collections.dll"),
				Path("System.Collections.Immutable.dll"),
				Path("System.Collections.NonGeneric.dll"),
				Path("System.Collections.Specialized.dll"),
				Path("System.ComponentModel.Annotations.dll"),
				Path("System.ComponentModel.DataAnnotations.dll"),
				Path("System.ComponentModel.dll"),
				Path("System.ComponentModel.EventBasedAsync.dll"),
				Path("System.ComponentModel.Primitives.dll"),
				Path("System.ComponentModel.TypeConverter.dll"),
				Path("System.Configuration.dll"),
				Path("System.Console.dll"),
				Path("System.Core.dll"),
				Path("System.Data.Common.dll"),
				Path("System.Data.DataSetExtensions.dll"),
				Path("System.Data.dll"),
				Path("System.Diagnostics.Contracts.dll"),
				Path("System.Diagnostics.Debug.dll"),
				Path("System.Diagnostics.DiagnosticSource.dll"),
				Path("System.Diagnostics.FileVersionInfo.dll"),
				Path("System.Diagnostics.Process.dll"),
				Path("System.Diagnostics.StackTrace.dll"),
				Path("System.Diagnostics.TextWriterTraceListener.dll"),
				Path("System.Diagnostics.Tools.dll"),
				Path("System.Diagnostics.TraceSource.dll"),
				Path("System.Diagnostics.Tracing.dll"),
				Path("System.dll"),
				Path("System.Drawing.dll"),
				Path("System.Drawing.Primitives.dll"),
				Path("System.Dynamic.Runtime.dll"),
				Path("System.Globalization.Calendars.dll"),
				Path("System.Globalization.dll"),
				Path("System.Globalization.Extensions.dll"),
				Path("System.IO.Compression.Brotli.dll"),
				Path("System.IO.Compression.dll"),
				Path("System.IO.Compression.FileSystem.dll"),
				Path("System.IO.Compression.ZipFile.dll"),
				Path("System.IO.dll"),
				Path("System.IO.FileSystem.dll"),
				Path("System.IO.FileSystem.DriveInfo.dll"),
				Path("System.IO.FileSystem.Primitives.dll"),
				Path("System.IO.FileSystem.Watcher.dll"),
				Path("System.IO.IsolatedStorage.dll"),
				Path("System.IO.MemoryMappedFiles.dll"),
				Path("System.IO.Pipes.dll"),
				Path("System.IO.UnmanagedMemoryStream.dll"),
				Path("System.Linq.dll"),
				Path("System.Linq.Expressions.dll"),
				Path("System.Linq.Parallel.dll"),
				Path("System.Linq.Queryable.dll"),
				Path("System.Memory.dll"),
				Path("System.Net.dll"),
				Path("System.Net.Http.dll"),
				Path("System.Net.HttpListener.dll"),
				Path("System.Net.Mail.dll"),
				Path("System.Net.NameResolution.dll"),
				Path("System.Net.NetworkInformation.dll"),
				Path("System.Net.Ping.dll"),
				Path("System.Net.Primitives.dll"),
				Path("System.Net.Requests.dll"),
				Path("System.Net.Security.dll"),
				Path("System.Net.ServicePoint.dll"),
				Path("System.Net.Sockets.dll"),
				Path("System.Net.WebClient.dll"),
				Path("System.Net.WebHeaderCollection.dll"),
				Path("System.Net.WebProxy.dll"),
				Path("System.Net.WebSockets.Client.dll"),
				Path("System.Net.WebSockets.dll"),
				Path("System.Numerics.dll"),
				Path("System.Numerics.Vectors.dll"),
				Path("System.ObjectModel.dll"),
				Path("System.Reflection.DispatchProxy.dll"),
				Path("System.Reflection.dll"),
				Path("System.Reflection.Emit.dll"),
				Path("System.Reflection.Emit.ILGeneration.dll"),
				Path("System.Reflection.Emit.Lightweight.dll"),
				Path("System.Reflection.Extensions.dll"),
				Path("System.Reflection.Metadata.dll"),
				Path("System.Reflection.Primitives.dll"),
				Path("System.Reflection.TypeExtensions.dll"),
				Path("System.Resources.Reader.dll"),
				Path("System.Resources.ResourceManager.dll"),
				Path("System.Resources.Writer.dll"),
				Path("System.Runtime.CompilerServices.Unsafe.dll"),
				Path("System.Runtime.CompilerServices.VisualC.dll"),
				Path("System.Runtime.dll"),
				Path("System.Runtime.Extensions.dll"),
				Path("System.Runtime.Handles.dll"),
				Path("System.Runtime.InteropServices.dll"),
				Path("System.Runtime.InteropServices.RuntimeInformation.dll"),
				Path("System.Runtime.InteropServices.WindowsRuntime.dll"),
				Path("System.Runtime.Intrinsics.dll"),
				Path("System.Runtime.Loader.dll"),
				Path("System.Runtime.Numerics.dll"),
				Path("System.Runtime.Serialization.dll"),
				Path("System.Runtime.Serialization.Formatters.dll"),
				Path("System.Runtime.Serialization.Json.dll"),
				Path("System.Runtime.Serialization.Primitives.dll"),
				Path("System.Runtime.Serialization.Xml.dll"),
				Path("System.Security.Claims.dll"),
				Path("System.Security.Cryptography.Algorithms.dll"),
				Path("System.Security.Cryptography.Csp.dll"),
				Path("System.Security.Cryptography.Encoding.dll"),
				Path("System.Security.Cryptography.Primitives.dll"),
				Path("System.Security.Cryptography.X509Certificates.dll"),
				Path("System.Security.dll"),
				Path("System.Security.Principal.dll"),
				Path("System.Security.SecureString.dll"),
				Path("System.ServiceModel.Web.dll"),
				Path("System.ServiceProcess.dll"),
				Path("System.Text.Encoding.CodePages.dll"),
				Path("System.Text.Encoding.dll"),
				Path("System.Text.Encoding.Extensions.dll"),
				Path("System.Text.Encodings.Web.dll"),
				Path("System.Text.Json.dll"),
				Path("System.Text.RegularExpressions.dll"),
				Path("System.Threading.Channels.dll"),
				Path("System.Threading.dll"),
				Path("System.Threading.Overlapped.dll"),
				Path("System.Threading.Tasks.Dataflow.dll"),
				Path("System.Threading.Tasks.dll"),
				Path("System.Threading.Tasks.Extensions.dll"),
				Path("System.Threading.Tasks.Parallel.dll"),
				Path("System.Threading.Thread.dll"),
				Path("System.Threading.ThreadPool.dll"),
				Path("System.Threading.Timer.dll"),
				Path("System.Transactions.dll"),
				Path("System.Transactions.Local.dll"),
				Path("System.ValueTuple.dll"),
				Path("System.Web.dll"),
				Path("System.Web.HttpUtility.dll"),
				Path("System.Windows.dll"),
				Path("System.Xml.dll"),
				Path("System.Xml.Linq.dll"),
				Path("System.Xml.ReaderWriter.dll"),
				Path("System.Xml.Serialization.dll"),
				Path("System.Xml.XDocument.dll"),
				Path("System.Xml.XmlDocument.dll"),
				Path("System.Xml.XmlSerializer.dll"),
				Path("System.Xml.XPath.dll"),
				Path("System.Xml.XPath.XDocument.dll"),
				Path("WindowsBase.dll"),
			});

			state.EnsureValue("PlatformLibraries").SetValuePathList(platformLibraries);
			state.EnsureValue("PlatformPreprocessorDefinitions").SetValueStringList(platformPreprocessorDefinitions);
		}

	private:
		Path FindVSInstallRoot(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			// Find a copy of visual studio that has the required VisualCompiler
			auto executablePath = Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			auto workingDirectory = Path("./");
			auto argumentList = std::vector<std::string>({
				"-latest",
				"-products",
				"*",
				"-requires",
				"Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
				"-property",
				"installationPath",
			});

			// Check if we should include pre-release versions
			bool includePrerelease = false;
			if (includePrerelease)
			{
				argumentList.push_back("-prerelease");
			}

			// Execute the requested target
			auto arguments = CombineArguments(argumentList);
			buildState.LogDebug(executablePath.ToString() + " " + arguments);
			auto process = System::IProcessManager::Current().CreateProcess(
				executablePath,
				arguments,
				workingDirectory);
			process->Start();
			process->WaitForExit();

			auto stdOut = process->GetStandardOutput();
			auto stdErr = process->GetStandardError();
			auto exitCode = process->GetExitCode();

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

			auto stream = std::istringstream(stdOut);

			// The first line is the path
			auto path = std::string();
			if (!std::getline(stream, path, '\r'))
			{
				buildState.LogError("Failed to parse vswhere output.");
				throw std::runtime_error("Failed to parse vswhere output.");
			}

			return Path(path);
		}

		std::string FindDefaultVCToolsVersion(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const Path& visualStudioInstallRoot)
		{
			// Check the default tools version
			auto visualCompilerToolsDefaultVersionFile =
				visualStudioInstallRoot + Path("VC/Auxiliary/Build/Microsoft.VCToolsVersion.default.txt");
			if (!System::IFileSystem::Current().Exists(visualCompilerToolsDefaultVersionFile))
			{
				buildState.LogError("VisualCompilerToolsDefaultVersionFile file does not exist: " + visualCompilerToolsDefaultVersionFile.ToString());
				throw std::runtime_error("VisualCompilerToolsDefaultVersionFile file does not exist.");
			}

			// Read the entire file into a string
			auto file = System::IFileSystem::Current().OpenRead(visualCompilerToolsDefaultVersionFile, false);

			// The first line is the version
			auto version = std::string();
			if (!std::getline(file->GetInStream(), version, '\n'))
			{
				buildState.LogError("Failed to parse version from file.");
				throw std::runtime_error("Failed to parse version from file.");
			}

			return version;
		}

		std::string FindNewestWindows10KitVersion(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			const Path& windows10KitIncludePath)
		{
			// Check the default tools version
			buildState.LogDebug("FindNewestWindows10KitVersion: " + windows10KitIncludePath.ToString());
			auto currentVersion = SemanticVersion(0, 0, 0);
			for (auto& child : System::IFileSystem::Current().GetDirectoryChildren(windows10KitIncludePath))
			{
				auto name = child.Path.GetFileName();
				buildState.LogDebug("CheckFile: " + std::string(name));
				auto platformVersion = name.substr(0, 3);
				if (platformVersion == "10.")
				{
					// Parse the version string
					auto version = SemanticVersion::Parse(name.substr(3));
					if (version > currentVersion)
						currentVersion = version;
				}
				else
				{
					buildState.LogWarning("Unexpected Kit Version: " + std::string(name));
				}
			}

			if (currentVersion == SemanticVersion(0, 0, 0))
				throw std::runtime_error("Could not find a minimum Windows 10 Kit Version");

			// The first line is the version
			auto version = "10." + currentVersion.ToString();
			return version;
		}

		static std::string CombineArguments(const std::vector<std::string>& args)
		{
			auto argumentString = std::stringstream();
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
		Soup::Build::Utilities::StringList _runBeforeList;
		Soup::Build::Utilities::StringList _runAfterList;
	};
}
