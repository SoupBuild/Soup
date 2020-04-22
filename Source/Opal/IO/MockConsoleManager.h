// <copyright file="MockConsoleManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IConsoleManager.h"
#include "MockConsoleInputStream.h"

namespace Opal::IO
{
	/// <summary>
	/// The mock console manager
	/// TODO: Move into test project
	/// </summary>
	export class MockConsoleManager : public IConsoleManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockConsoleManager'/> class.
		/// </summary>
		MockConsoleManager() :
			_requests(),
			_standardInput(std::make_shared<MockConsoleInputStream>(_requests))
		{
		}

		/// <summary>
		/// Get the requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Get the mock input stream
		/// </summary>
		MockConsoleInputStream& GetMockInputStream()
		{
			return *_standardInput;
		}

		/// <summary>
		/// Get the standard input stream
		/// </summary>
		std::shared_ptr<IConsoleInputStream> GetStandardInput() override final
		{
			auto message = std::stringstream();
			message << "GetStandardInput";
			_requests.push_back(message.str());

			return _standardInput;
		}

	private:
		std::vector<std::string> _requests;
		std::shared_ptr<MockConsoleInputStream> _standardInput;
	};
}
