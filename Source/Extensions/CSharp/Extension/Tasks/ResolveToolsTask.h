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
			auto visualStudioInstallRoot = FindVSPathRoot(buildState);
			buildState.LogInfo("Using VS Installation: " + visualStudioInstallRoot.ToString());

			// Calculate the final CSC tools folder
			auto rosylnBinFolder =
				visualStudioInstallRoot + Path("/MSBuild/Current/Bin/Roslyn/");

			// Save the build properties
			state.EnsureValue("Roslyn.InstallRoot").SetValueString(visualStudioInstallRoot.ToString());
			state.EnsureValue("Roslyn.BinaryRoot").SetValueString(rosylnBinFolder.ToString());
		}

	private:
		Path FindVSPathRoot(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			// Find a copy of visual studio that has the required VisualCompiler
			auto executablePath = Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			auto workingDirectory = Path("./");
			auto argumentList = std::vector<std::string>({
				"-latest",
				"-requires",
				"Microsoft.VisualStudio.Component.Roslyn.Compiler",
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
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
