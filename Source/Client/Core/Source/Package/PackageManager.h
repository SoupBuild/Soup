// <copyright file="PackageManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The package manager
	/// </summary>
	export class PackageManager
	{
	public:
		/// <summary>
		/// Install packages
		/// </summary>
		static void InstallPackages(const Path& workingDirectory)
		{
			Log::Info("InstallPackages");

			auto arguments = std::stringstream();
			arguments << "install-packages " << workingDirectory.ToString();

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
				packageManagerFolder);
			process->Start();
			process->WaitForExit();

			auto stdOut = process->GetStandardOutput();
			auto stdErr = process->GetStandardError();
			auto exitCode = process->GetExitCode();

			// TODO: Directly pipe to output and make sure there is no extra newline
			if (!stdOut.empty())
			{
				Log::HighPriority(stdOut);
			}

			if (!stdErr.empty())
			{
				Log::Error(stdErr);
			}

			if (exitCode != 0)
				throw HandledException(exitCode);
		}
	};
}
