// <copyright file="MockEnvironment.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IEnvironment.h"

namespace Opal::System
{
	/// <summary>
	/// The mock environment
	/// TODO: Move into test project
	/// </summary>
	export class MockEnvironment : public IEnvironment
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockEnvironment'/> class.
		/// </summary>
		MockEnvironment() :
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
		/// Gets the value of an environment variable
		/// </summary>
		std::string GetEnvironmentVariable(std::string_view name) override final
		{
			std::stringstream message;
			message << "GetEnvironmentVariable: " << name;

			_requests.push_back(message.str());
			return "value";
		}

	private:
		std::vector<std::string> _requests;
	};
}
