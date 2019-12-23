// <copyright file="MockProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Dubious::System
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
			_processFileName(Path("C:/testlocation/SoupCMDTest.exe")),
			_requests()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		MockProcessManager(Path processFileName) :
			_processFileName(std::move(processFileName)),
			_requests()
		{
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
			const std::vector<std::string>& arguments,
			const Path& workingDirectory) override final
		{
			std::stringstream message;
			message << "Execute: " << workingDirectory.ToString() << ": " << application.ToString();
			for (auto& value : arguments)
				message << " " << value;

			_requests.push_back(message.str());
			return {
				0,
				std::string(),
				std::string(),
			};
		}

	private:
		Path _processFileName;
		std::vector<std::string> _requests;
	};
}
