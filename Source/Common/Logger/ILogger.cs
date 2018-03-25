// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;

namespace Soup
{
	public interface ILogger
	{
		void Message(string message);

		void Message(string message, ConsoleColor color);

		void Verbose(string message);

		void Warning(string message);

		void Error(string message);
	}
}
