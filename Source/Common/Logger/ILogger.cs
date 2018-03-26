// <copyright company="Soup" file="ILogger.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;

	public interface ILogger
	{
		void Message(string message);

		void Message(string message, ConsoleColor color);

		void Verbose(string message);

		void Warning(string message);

		void Error(string message);
	}
}
