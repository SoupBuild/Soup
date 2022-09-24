
using Opal;
using Opal.System;
using System.Threading.Tasks;

namespace Soup.Build.Generate
{
	class Program
	{
		static async Task<int> Main(string[] args)
		{
			// Register the runtime services
			Log.RegisterListener(new ConsoleTraceListener());
			LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

			if (args.Length != 1)
			{
				Log.Error("Invalid parameters. Expected one parameter.");
				return -1;
			}

			var soupTargetDirectory = new Path(args[0]);
			var generateEngine = new BuildGenerateEngine();
			await generateEngine.GenerateAsync(soupTargetDirectory);

			return 0;
		}
	}
}
