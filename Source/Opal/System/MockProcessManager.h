// <copyright file="MockProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Opal::System
{
	/// <summary>
	/// The mock process manager
	/// TODO: Move into test project
	/// </summary>
	export class MockProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		MockProcessManager() :
			_requests(),
			_processFileName(Path("C:/testlocation/SoupCMDTest.exe")),
			_executeResults()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		MockProcessManager(Path processFileName) :
			_requests(),
			_processFileName(std::move(processFileName)),
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
		/// Gets the process file name
		/// </summary>
		Path GetProcessFileName() override final
		{
			std::stringstream message;
			message << "GetProcessFileName";

			_requests.push_back(message.str());
			return _processFileName;
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		ProcessResult Execute(
			const Path& application,
			const std::string& arguments,
			const Path& workingDirectory) override final
		{
			std::stringstream message;
			message << "Execute: [" << workingDirectory.ToString() << "] " << application.ToString() << " " << arguments;

			_requests.push_back(message.str());

			// Check if there is a registered output
			auto findOutput = _executeResults.find(message.str());
			if (findOutput != _executeResults.end())
			{
				return {
					0,
					findOutput->second,
					std::string(),
				};
			}
			else
			{
				return {
					0,
					std::string(),
					std::string(),
				};
			}
		}

	private:
		std::vector<std::string> _requests;
		Path _processFileName;
		std::map<std::string, std::string> _executeResults;
	};
}
