// <copyright file="TestTraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "TraceListener.h"

namespace Soup
{
	/// <summary>
	/// Test logger that wraps the base <see cref="TraceListener"/>
	/// </summary>
	export class TestTraceListener : public TraceListener
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='TestTraceListener'/> class.
		/// </summary>
		TestTraceListener() :
			TraceListener("", nullptr, true, false),
			_messages()
		{
		}

		/// <summary>
		/// Get the messages
		/// </summary>
		const std::vector<std::string>& GetMessages() const
		{
			return _messages;
		}

		/// <summary>
		/// Writes a message and newline terminator
		/// </summary>
		virtual void WriteLine(std::string_view message) override final
		{
			auto line = std::string(message);
			_messages.push_back(std::move(line));
		}

	private:
		std::vector<std::string> _messages;
	};
}
