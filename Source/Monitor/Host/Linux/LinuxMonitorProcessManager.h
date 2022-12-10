// <copyright file="LinuxProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourProcessManager.h"

namespace Monitor
{
	/// <summary>
	/// A Linux platform specific process executable using system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class LinuxDetourProcessManager : public IDetourProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxDetourProcessManager'/> class.
		/// </summary>
		LinuxDetourProcessManager()
		{
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		std::shared_ptr<Opal::System::IProcess> CreateDetourProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			const std::map<std::string, std::string>& environmentVariables,
			std::shared_ptr<IDetourCallback> callback,
			bool enableAccessChecks,
			const std::vector<Path>& allowedReadAccess,
			const std::vector<Path>& allowedWriteAccess) override final
		{
			throw std::runtime_error("Not implemented");
		}
	};
}
