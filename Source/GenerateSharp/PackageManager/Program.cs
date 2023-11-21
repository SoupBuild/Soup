// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager
{
	using System;
	using System.Net.Http;
	using System.Threading.Tasks;
	using Opal;
	using Opal.System;

	public class Program
	{
		//// private static Uri SoupApiEndpoint => new Uri("http://localhost:7070");
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
				Log.RegisterListener(new ConsoleTraceListener("", new EventTypeFilter(traceFlags), false, false));
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
				SemanticVersion builtInLanguageVersionCSharp = new SemanticVersion(0, 9, 0);
				SemanticVersion builtInLanguageVersionCpp = new SemanticVersion(0, 8, 2);
				SemanticVersion builtInLanguageVersionWren = new SemanticVersion(0, 2, 0);
				var closureManager = new ClosureManager(
					SoupApiEndpoint,
					httpClient,
					builtInLanguageVersionCSharp,
					builtInLanguageVersionCpp,
					builtInLanguageVersionWren);
				var packageManager = new PackageManager(
					SoupApiEndpoint,
					httpClient,
					closureManager);

				switch (command)
				{
					case "initialize-package":
						{
							var initializeCommand = new InitializeCommand(builtInLanguageVersionCpp);

							await initializeCommand.InitializePackageAsync(workingDirectory);
						}
						break;
					case "restore-packages":
						{
							bool forceRestore = false;
							if (args.Length == 3 && args[2] == "--force")
							{
								forceRestore = true;
							}
							else if (args.Length != 2)
							{
								PrintUsage();
								return -1;
							}

							await packageManager.RestorePackagesAsync(workingDirectory, forceRestore);
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
			catch (Exception ex)
			{
				Log.HighPriority($"Unhandled Error: {ex}");
				return -2;
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
}
