// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;

namespace Soup
{
	public static class Log
	{
		public static void WriteLine(string message)
		{
			Console.WriteLine(message);
		}

		public static void WriteLine(string message, ConsoleColor color)
		{
			var previousForegroundColor = Console.ForegroundColor;
			Console.ForegroundColor = color;
			WriteLine(message);
			Console.ForegroundColor = previousForegroundColor;
		}

		public static void Message(string message)
		{
			WriteLine(message);
		}

		public static void Verbose(string message)
		{
			WriteLine(message);
		}

		public static void Warning(string message)
		{
			WriteLine($"WARNING: {message}", ConsoleColor.Yellow);
		}

		public static void Error(string message)
		{
			WriteLine($"ERROR: {message}", ConsoleColor.Red);
		}
	}
}
