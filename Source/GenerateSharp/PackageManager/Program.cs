// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager
{
    using System.Threading.Tasks;
    using Opal;
    using Opal.System;

    public class Program
    {
        public static async Task<int> Main(string[] args)
        {
            Log.RegisterListener(new ConsoleTraceListener("", new EventTypeFilter(TraceEventFlag.Information), false, false));
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

        private static void PrintUsage()
        {
            Log.Info("Soup.Build.PackageManager.exe [command] [directory]");
            Log.Info("\tinstall-package [directory] [packageReference]");
            Log.Info("\tinstall-packages [directory]");
        }
    }
}
