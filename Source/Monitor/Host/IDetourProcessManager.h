// <copyright file="IDetourProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourCallback.h"

namespace Monitor
{
	/// <summary>
	/// The process manager interface that supports detour callbacks
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class IDetourProcessManager
	{
	public:
		/// <summary>
		/// Gets the current active process manager
		/// </summary>
		static IDetourProcessManager& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No detour process manager implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active process manager
		/// </summary>
		static void Register(std::shared_ptr<IDetourProcessManager> value)
		{
			_current = std::move(value);
		}

	public:
		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		virtual std::shared_ptr<Opal::System::IProcess> CreateDetourProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			const std::map<std::string, std::string>& environmentVariables,
			std::shared_ptr<IDetourCallback> callback,
			bool enableAccessChecks,
			const std::vector<Path>& allowedReadAccess,
			const std::vector<Path>& allowedWriteAccess) = 0;

	private:
		static std::shared_ptr<IDetourProcessManager> _current;
	};

	std::shared_ptr<IDetourProcessManager> IDetourProcessManager::_current = nullptr;
}
