// <copyright file="ICommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Client
{
	/// <summary>
	/// Command Interface
	/// </summary>
	class ICommand
	{
	public:
		/// <summary>
		/// Main entry point for a unique command
		/// </summary>
		virtual void Run() = 0;
	};
}
