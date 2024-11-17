// <copyright file="WindowsMonitorProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IMonitorProcessManager.h"
#include "WindowsMonitorProcess.h"

namespace Monitor::Windows
{
	/// <summary>
	/// A windows platform specific process executable using system
	/// </summary>
	export class WindowsMonitorProcessManager : public IMonitorProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsMonitorProcessManager'/> class.
		/// </summary>
		WindowsMonitorProcessManager()
		{
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		std::shared_ptr<Opal::System::IProcess> CreateMonitorProcess(
			const Path& executable,
			std::vector<std::string> arguments,
			const Path& workingDirectory,
			const std::map<std::string, std::string>& environmentVariables,
			std::shared_ptr<ISystemAccessMonitor> monitor,
			bool enableAccessChecks,
			std::vector<Path> allowedReadAccess,
			std::vector<Path> allowedWriteAccess) override final
		{
			return std::make_shared<WindowsMonitorProcess>(
				executable,
				std::move(arguments),
				workingDirectory,
				environmentVariables,
				std::move(monitor),
				enableAccessChecks,
				std::move(allowedReadAccess),
				std::move(allowedWriteAccess));
		}
	};
}
