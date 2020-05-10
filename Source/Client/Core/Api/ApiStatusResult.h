// <copyright file="ApiStatusResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api
{
	/// <summary>
	/// Api Status Result
	/// </summary>
	template<typename TStatus, typename TResult>
	class ApiStatusResult
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="ApiStatusResult"/> class.
		/// </summary>
		ApiStatusResult(TStatus status, TResult result) :
			Status(status),
			Result(std::move(result))
		{
		}

		TStatus Status;
		TResult Result;
	};
}
