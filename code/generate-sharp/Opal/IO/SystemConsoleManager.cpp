// <copyright file="SystemConsoleManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IConsoleManager.h"
#include "SystemConsoleInputStream.h"

namespace Opal::IO
{
	/// <summary>
	/// The system console manager
	/// </summary>
	export class SystemConsoleManager : public IConsoleManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='SystemConsoleManager'/> class.
		/// </summary>
		SystemConsoleManager()
		{
		}

		/// <summary>
		/// Get the standard input stream
		/// </summary>
		virtual std::shared_ptr<IConsoleInputStream> GetStandardInput() override final
		{
			return std::make_shared<SystemConsoleInputStream>();
		}
	};
}
