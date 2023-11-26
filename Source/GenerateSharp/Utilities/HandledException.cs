// <copyright file="HandledException.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build;

/// <summary>
/// An exception type that indicates an error was already handled and we can safely exit
/// </summary>
public class HandledException : Exception
{
	public HandledException(string message) : base(message)
	{
	}

	public HandledException(string message, Exception innerException) : base(message, innerException)
	{
	}

	public HandledException()
	{
	}
}
