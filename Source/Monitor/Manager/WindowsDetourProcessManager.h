// <copyright file="WindowsProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourProcessManager.h"
#include "WindowsDetourProcess.h"

namespace Monitor
{
	/// <summary>
	/// A windows splatform specific process executable using system
	/// </summary>
	export class WindowsDetourProcessManager : public IDetourProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsDetourProcessManager'/> class.
		/// </summary>
		WindowsDetourProcessManager()
		{
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		std::shared_ptr<Opal::System::IProcess> CreateDetourProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			std::shared_ptr<IDetourCallback> callback) override final
		{
			return std::make_shared<WindowsDetourProcess>(
				executable,
				arguments,
				workingDirectory,
				std::move(callback));
		}
	};
}
