// <copyright file="HandledException.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// A special exception overload that indicates an early exit for the application that was handled
	/// </summary>
	export class HandledException : std::exception
	{
	public:
		/// <summary>
		/// Initialize a new instance of the HandledException class
		/// </summary>
		HandledException() :
			std::exception()
		{
		}
	};
}
