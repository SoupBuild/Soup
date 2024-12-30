// <copyright file="BuildFailedException.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The exception thrown to indcate that a build operation has failed
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class BuildFailedException : public std::exception
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildFailedException"/> class.
		/// </summary>
		BuildFailedException()
		{
		}
	};
}
