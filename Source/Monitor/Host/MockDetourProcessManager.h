// <copyright file="MockDetourProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourProcessManager.h"

namespace Monitor
{
	/// <summary>
	/// The mock detour process manager
	/// TODO: Move into test project
	/// </summary>
	export class MockDetourProcessManager : public IDetourProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockDetourProcessManager'/> class.
		/// </summary>
		MockDetourProcessManager() :
			m_uniqueId(1),
			_requests(),
			_executeResults()
		{
		}

		/// <summary>
		/// Create a result 
		/// </summary>
		void RegisterExecuteResult(std::string command, std::string output)
		{
			_executeResults.emplace(
				std::move(command),
				std::move(output));
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
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
			std::stringstream message;
			auto id = m_uniqueId++;
			message << "CreateDetourProcess: " << id << " [" << workingDirectory.ToString() << "] " << executable.ToString() << " " << arguments;
			message << " Environment [" << environmentVariables.size() << "]";
			message << " " << enableAccessChecks;
			message << " AllowedRead [" << allowedReadAccess.size() << "]";
			message << " AllowedWrite [" << allowedWriteAccess.size() << "]";

			_requests.push_back(message.str());

			// Check if there is a registered output
			auto findOutput = _executeResults.find(message.str());
			if (findOutput != _executeResults.end())
			{
				return std::make_shared<Opal::System::MockProcess>(
					id,
					_requests,
					0,
					findOutput->second,
					std::string());
			}
			else
			{
				return std::make_shared<Opal::System::MockProcess>(
					id,
					_requests,
					0,
					std::string(),
					std::string());
			}
		}

	private:
		std::atomic<int> m_uniqueId;
		std::vector<std::string> _requests;
		std::map<std::string, std::string> _executeResults;
	};
}
