// <copyright file="LinuxMonitorProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "../IMonitorProcessManager.h"
#include "LinuxMonitorProcess.h"

namespace Monitor::Linux
{
	/// <summary>
	/// A Linux platform specific process executable using system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class LinuxMonitorProcessManager : public IMonitorProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxMonitorProcessManager'/> class.
		/// </summary>
		LinuxMonitorProcessManager()
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
			std::shared_ptr<IMonitorCallback> callback,
			bool enableAccessChecks,
			const std::vector<Path>& allowedReadAccess,
			const std::vector<Path>& allowedWriteAccess) override final
		{
			return std::make_shared<LinuxMonitorProcess>(
				executable,
				std::move(arguments),
				workingDirectory,
				std::move(callback));
		}
	};
}
