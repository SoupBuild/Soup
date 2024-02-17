// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace Soup.Build.PackageManager;

public static class Program
{
	// private static Uri SoupApiEndpoint => new Uri("https://localhost:7070");
	private static Uri SoupApiEndpoint => new Uri("https://api.soupbuild.com");

	public static async Task<int> Main(string[] args)
	{
		try
		{
			var traceFlags =
				TraceEvents.Information |
				TraceEvents.HighPriority |
				TraceEvents.Critical |
				TraceEvents.Warning |
				TraceEvents.Error;
			Log.RegisterListener(new ConsoleTraceListener(new EventTypeFilter(traceFlags), false, false));
			LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();
			LifetimeManager.RegisterSingleton<IAuthenticationManager, AuthenticationManager>();
			LifetimeManager.RegisterSingleton<IZipManager, CompressionZipManager>();

			if (args.Length < 2)
			{
				PrintUsage();
				return -1;
			}

			var command = args[0];
			var workingDirectory = new Path(args[1]);

			using var httpClient = new HttpClient();
			SemanticVersion minimumLanguageVersionCpp = new SemanticVersion(0, 12, 0);
			SemanticVersion builtInLanguageVersionWren = new SemanticVersion(0, 4, 1);
			var closureManager = new ClosureManager(
				SoupApiEndpoint,
				httpClient,
				builtInLanguageVersionWren);
			var packageManager = new PackageManager(
				SoupApiEndpoint,
				httpClient,
				closureManager);

			switch (command)
			{
				case "initialize-package":
					{
						var initializeCommand = new InitializeCommand(minimumLanguageVersionCpp);

						await initializeCommand.InitializePackageAsync(workingDirectory);
					}
					break;
				case "restore-packages":
					{
						if (args.Length != 2)
						{
							PrintUsage();
							return -1;
						}

						await packageManager.RestorePackagesAsync(workingDirectory);
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
						await packageManager.InstallPackageReferenceAsync(workingDirectory, packageReference);
					}
					break;
				case "publish-package":
					{
						if (args.Length != 2)
						{
							PrintUsage();
							return -1;
						}

						await packageManager.PublishPackageAsync(workingDirectory);
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
	}

	private static void PrintUsage()
	{
		Log.Info("Soup.Build.PackageManager.exe [command] [directory]");
		Log.Info("\tinstall-package [directory] [packageReference]");
		Log.Info("\trestore-packages [directory]");
		Log.Info("\tpublish-package [directory]");
	}
}