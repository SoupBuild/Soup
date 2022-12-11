// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The package manager wrapper that sends requests along to the managed implementation
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class PackageManager
	{
	public:
		/// <summary>
		/// Restore packages
		/// </summary>
		static void RestorePackages(const Path& workingDirectory)
		{
			Log::Info("RestorePackages");

			auto arguments = std::stringstream();
			arguments << "restore-packages " << workingDirectory.ToString();

			RunCommand(arguments.str());
		}

		/// <summary>
		/// Initialize a package
		/// </summary>
		static void InitializePackage(const Path& workingDirectory)
		{
			Log::Info("InitializePackage");

			auto arguments = std::stringstream();
			arguments << "initialize-package " << workingDirectory.ToString();

			RunCommand(arguments.str());
		}

		/// <summary>
		/// Install a package
		/// </summary>
		static void InstallPackageReference(const Path& workingDirectory, const std::string& packageReference)
		{
			Log::Info("InstallPackageReference");

			auto arguments = std::stringstream();
			arguments << "install-package " << workingDirectory.ToString() << " " << packageReference;

			RunCommand(arguments.str());
		}

		/// <summary>
		/// Publish a package
		/// </summary>
		static void PublishPackage(const Path& workingDirectory)
		{
			Log::Info("PublishPackage");

			auto arguments = std::stringstream();
			arguments << "publish-package " << workingDirectory.ToString();

			RunCommand(arguments.str());
		}

	private:
		static void RunCommand(std::string arguments)
		{
			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto packageManagerFolder = moduleFolder + Path("PackageManager/");
			auto executable = packageManagerFolder + Path("Soup.Build.PackageManager.exe");

			// Execute the requested target
			Log::Info("Running PackageManager: " + arguments);
			auto process = System::IProcessManager::Current().CreateProcess(
				executable,
				arguments,
				packageManagerFolder,
				false);
			process->Start();
			process->WaitForExit();

			auto exitCode = process->GetExitCode();

			if (exitCode != 0)
				throw HandledException(exitCode);
		}
	};
}
