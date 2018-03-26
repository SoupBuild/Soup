// <copyright company="Soup" file="Log.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;

	public static class Log
	{
		public static void Message(string message)
		{
			Singleton<ILogger>.Instance.Message(message);
		}

		public static void Message(string message, ConsoleColor color)
		{
			Singleton<ILogger>.Instance.Message(message, color);
		}

		public static void Verbose(string message)
		{
			Singleton<ILogger>.Instance.Verbose(message);
		}

		public static void Warning(string message)
		{
			Singleton<ILogger>.Instance.Warning(message);
		}

		public static void Error(string message)
		{
			Singleton<ILogger>.Instance.Error(message);
		}
	}
}
