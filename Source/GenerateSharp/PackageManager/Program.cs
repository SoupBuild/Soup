// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager
{
	using System;
	using System.Threading.Tasks;
	using Opal;
	using Opal.System;

	public class Program
	{
		public static async Task<int> Main(string[] args)
		{
			try
			{
				var traceFlags =
					TraceEventFlag.Information |
					TraceEventFlag.HighPriority |
					TraceEventFlag.Critical |
					TraceEventFlag.Warning |
					TraceEventFlag.Error;
				Log.RegisterListener(new ConsoleTraceListener("", new EventTypeFilter(traceFlags), false, false));
				LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

				if (args.Length < 2)
				{
					PrintUsage();
					return -1;
				}

				var command = args[0];
				var workingDirectory = new Path(args[1]);

				switch (command)
				{
					case "install-packages":
						{
							if (args.Length != 2)
							{
								PrintUsage();
								return -1;
							}

							await PackageManager.InstallPackagesAsync(workingDirectory);
						}
						break;
					case "install-package":
						{
							if (args.Length != 3)
							{
								PrintUsage();
								return -1;
							}

							var packageReference = args[2];
							await PackageManager.InstallPackageReferenceAsync(workingDirectory, packageReference);
						}
						break;
					case "publish-package":
						{
							if (args.Length != 2)
							{
								PrintUsage();
								return -1;
							}

							await PackageManager.PublishPackageAsync(workingDirectory);
						}
						break;
					default:
						PrintUsage();
						return -1;
				}

				return 0;
			}
			catch (HandledException)
			{
				return -1;
			}
			catch (Exception ex)
			{
				Log.HighPriority($"Unhandled error {ex.Message}");
				return -2;
			}
		}

		private static void PrintUsage()
		{
			Log.Info("Soup.Build.PackageManager.exe [command] [directory]");
			Log.Info("\tinstall-package [directory] [packageReference]");
			Log.Info("\tinstall-packages [directory]");
		}
	}
}
