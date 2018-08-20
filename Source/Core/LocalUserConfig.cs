// <copyright company="Soup" file="LocalUserConfig.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;
	using System.IO;
	using System.Runtime.InteropServices;

	public class LocalUserConfig
	{
		public string PackageStore
		{
			get 
			{
				if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
				{
					return @"C:\SoupKitchen";
				}
				else
				{
					var homePath = Environment.GetEnvironmentVariable("HOME");
					return Path.Combine(homePath, ".soup");
				}
			}
		}
	}
}
