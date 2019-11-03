// <copyright file="PlatformProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Soup
{
	/// <summary>
	/// A platform specific process executable using system
	/// </summary>
	export class PlatformProcessManager : public IProcessManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='PlatformProcessManager'/> class.
		/// </summary>
		PlatformProcessManager()
		{
		}

		/// <summary>
		/// Gets the process file name
		/// </summary>
		Path GetProcessFileName() override final
		{
			try
			{
				auto buffer = std::array<char, 1024>();
				Platform::ProcessManager::GetProcessFileName(buffer.data(), buffer.size());
				return Path(std::string(buffer.data(), buffer.size()));
			}
			catch (unsigned long error)
			{
				// Convert raw error into a std exception
				throw std::runtime_error("GetProcessFileName Failed: " + std::to_string(error));
			}
			catch (const char* message)
			{
				// Convert raw message into a std exception
				throw std::runtime_error(message);
			}
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		ProcessResult Execute(
			const Path& application,
			const std::vector<std::string>& arguments,
			const Path& workingDirectory) override final
		{
			std::stringstream argumentsValue;
			argumentsValue << "\"" << application.ToString() << "\"";
			for (auto& value : arguments)
				argumentsValue << " " << value;

			std::string argumentsString = argumentsValue.str();
			Log::Verbose(workingDirectory.ToString() + ": " + argumentsString);

			// Aggregate all the output
			std::stringstream stdOut;
			std::stringstream stdErr;

			// Convert to c style since we do not have access to stdlib in this helper
			// TODO: Remove crazyness when we get a good implementation of modules
			auto outputCallback = [](void* context, const char* value, int length) 
			{ 
				auto& stdOut = *reinterpret_cast<std::stringstream*>(context);
				stdOut << value;
			};
			auto errorCallback = [](void* context, const char* value, int length)
			{
				auto& stdErr = *reinterpret_cast<std::stringstream*>(context);
				stdErr << value;
			};

			int exitCode = Platform::ProcessManager::Execute(
				application.ToString().c_str(),
				const_cast<char*>(argumentsString.c_str()),
				workingDirectory.ToString().c_str(),
				outputCallback,
				&stdOut,
				errorCallback,
				&stdErr);

			return {
				exitCode,
				stdOut.str(),
				stdErr.str(),
			};
		}
	};
}
