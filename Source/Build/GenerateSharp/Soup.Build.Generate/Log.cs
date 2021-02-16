using System;

namespace Soup.Build.Generate
{
	internal static class Log
	{
		public static void Info(string message)
		{
			Console.WriteLine(message);
		}
		public static void HighPriority(string message)
		{
			Console.WriteLine(message);
		}

		public static void Diag(string message)
		{
			Console.WriteLine(message);
		}
		public static void Warning(string message)
		{
			Console.WriteLine(message);
		}

		public static void Error(string message)
		{
			Console.WriteLine(message);
		}
	}
}
