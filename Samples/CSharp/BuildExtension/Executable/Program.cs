// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace BuildExtension.Executable
{
	using System;

	public class Program
	{
		public static void Main(string[] args)
		{
			#if SPECIAL_BUILD
				Console.WriteLine("Hello World, Soup Style!");
			#else
				Console.WriteLine("Hello World...");
			#endif
		}
	}
}
