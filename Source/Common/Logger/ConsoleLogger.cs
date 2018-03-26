// <copyright company="Soup" file="ConsoleLogger.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;

	public class ConsoleLogger : ILogger
	{
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
	}
}
