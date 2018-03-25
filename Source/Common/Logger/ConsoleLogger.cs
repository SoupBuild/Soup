// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;

namespace Soup
{
	public class ConsoleLogger : ILogger
	{
		private void WriteLine(string message)
		{
			Console.WriteLine(message);
		}

		private void WriteLine(string message, ConsoleColor color)
		{
			var previousForegroundColor = Console.ForegroundColor;
			Console.ForegroundColor = color;
			WriteLine(message);
			Console.ForegroundColor = previousForegroundColor;
		}

		public void Message(string message)
		{
			WriteLine(message);
		}

		public void Message(string message, ConsoleColor color)
		{
			WriteLine(message, color);
		}

		public void Verbose(string message)
		{
			WriteLine(message);
		}

		public void Warning(string message)
		{
			WriteLine($"WARNING: {message}", ConsoleColor.Yellow);
		}

		public void Error(string message)
		{
			WriteLine($"ERROR: {message}", ConsoleColor.Red);
		}
	}
}
