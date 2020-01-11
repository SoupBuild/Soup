// <copyright file="ResolveToolsTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild
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
		/// The Core Execute task
		/// </summary>
		Soup::Build::OperationResult Execute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::BuildStateWrapper(buildState);

			try
			{
				return Execute(buildStateWrapper);
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return -3;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::OperationResult Execute(
			Soup::Build::BuildStateWrapper& buildState)
		{
			try
			{
				auto state = buildState.GetActiveState();

				// Find the location of the Windows SDK
				auto visualStudioInstallRoot = FindVSInstallRoot(buildState);
				buildState.LogInfo("Using VS Installation: " + visualStudioInstallRoot.ToString());

				// Use the default version
				auto visualCompilerVersion = FindDefaultVCToolsVersion(buildState, visualStudioInstallRoot);
				buildState.LogInfo("Using VC Version: " + visualCompilerVersion);

				// Calculate the final VC tools folder
				auto visualCompilerVersionFolder =
					visualStudioInstallRoot + Path("/VC/Tools/MSVC/") + Path(visualCompilerVersion);

				// Save the build properties
				state.SetPropertyStringValue("MSVS.InstallRoot", visualStudioInstallRoot.ToString());
				state.SetPropertyStringValue("MSVC.Version", visualCompilerVersion);
				state.SetPropertyStringValue("MSVC.VCToolsRoot", visualCompilerVersionFolder.ToString());

				return 0;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		Path FindVSInstallRoot(Soup::Build::BuildStateWrapper& buildState)
		{
			// Find a copy of visual studio that has the required VisualCompiler
			auto executablePath = Path("C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe");
			auto workingDirectory = Path("");
			auto arguments = std::vector<std::string>({
				"-latest",
				"-products",
				"*",
				"-requires",
				"Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
				"-property",
				"installationPath",
			});

			// Execute the requested target
			auto result = System::IProcessManager::Current().Execute(
				executablePath,
				CombineArguments(arguments),
				workingDirectory);

			if (!result.StdErr.empty())
			{
				buildState.LogError(result.StdErr);
				throw std::runtime_error("VSWhere failed.");
			}

			if (result.ExitCode != 0)
			{
				// TODO: Return error code
				buildState.LogError("FAILED");
				throw std::runtime_error("VSWhere failed.");
			}

			auto stream = std::istringstream(result.StdOut);

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
			Soup::Build::BuildStateWrapper& buildState,
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
			auto stream = System::IFileSystem::Current().OpenRead(visualCompilerToolsDefaultVersionFile);

			// The first line is the version
			auto version = std::string();
			if (!std::getline(*stream, version, '\n'))
			{
				buildState.LogError("Failed to parse version from file.");
				throw std::runtime_error("Failed to parse version from file.");
			}

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
	};
}
