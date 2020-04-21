// <copyright file="MockConsoleInputStream.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IConsoleInputStream.h"

namespace Opal::IO
{
	/// <summary>
	/// A mock Console Input Stream
	/// </summary>
	export class MockConsoleInputStream : public IConsoleInputStream
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockConsoleInputStream'/> class.
		/// </summary>
		MockConsoleInputStream(std::vector<std::string>& parentRequests) :
			_parentRequests(parentRequests),
			_readPasswordResponse(),
			_readLineResponse()
		{
		}

		/// <summary>
		/// Set the response for a read password request
		/// </summary>
		void SetReadPasswordResponse(std::string response)
		{
			_readPasswordResponse = response;
		}

		/// <summary>
		/// Set the response for a read line request
		/// </summary>
		void SetReadLineResponse(std::string response)
		{
			_readLineResponse = response;
		}

		/// <summary>
		/// Read a line from the stream
		/// </summary>
		virtual std::string ReadLine() override final
		{
			auto message = std::stringstream();
			message << "ReadLine: " << _readLineResponse;
			_parentRequests.push_back(message.str());

			return _readLineResponse;
		}

		/// <summary>
		/// Read a password from the stream
		/// </summary>
		virtual std::string ReadPassword() override final
		{
			auto message = std::stringstream();
			message << "ReadPassword: " << _readPasswordResponse;
			_parentRequests.push_back(message.str());

			return _readPasswordResponse;
		}

	private:
		std::vector<std::string>& _parentRequests;
		std::string _readPasswordResponse;
		std::string _readLineResponse;
	};
}
