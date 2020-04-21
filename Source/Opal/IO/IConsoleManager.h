// <copyright file="IConsoleManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IConsoleInputStream.h"

namespace Opal::IO
{
	/// <summary>
	/// The input manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IConsoleManager
	{
	public:
		/// <summary>
		/// Gets the current active manager
		/// </summary>
		static IConsoleManager& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No console manager implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active input manager
		/// </summary>
		static void Register(std::shared_ptr<IConsoleManager> manager)
		{
			_current = std::move(manager);
		}

	public:
		/// <summary>
		/// Get the standard input stream
		/// </summary>
		virtual std::shared_ptr<IConsoleInputStream> GetStandardInput() = 0;

	private:
		static std::shared_ptr<IConsoleManager> _current;
	};

	std::shared_ptr<IConsoleManager> IConsoleManager::_current = nullptr;
}
