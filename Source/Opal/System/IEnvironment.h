// <copyright file="IProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// The environment interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class IEnvironment
	{
	public:
		/// <summary>
		/// Gets the current active environments
		/// </summary>
		static IEnvironment& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No environment implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active process manager
		/// </summary>
		static void Register(std::shared_ptr<IEnvironment> value)
		{
			_current = std::move(value);
		}

	public:
		/// <summary>
		/// Gets the value of an environment variable
		/// </summary>
		virtual std::string GetEnvironmentVariable(std::string_view name) = 0;

	private:
		static std::shared_ptr<IEnvironment> _current;
	};

	std::shared_ptr<IEnvironment> IEnvironment::_current = nullptr;
}
