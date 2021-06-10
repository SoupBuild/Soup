
using Opal;

namespace Soup.Build.Generate
{
	class Program
	{
		static int Main(string[] args)
		{
			// Register the runtime services
			Log.RegisterListener(new ConsoleTraceListener());
			// IFileSystem.Register(new RuntimeFileSystem());

			if (args.Length != 1)
			{
				Log.Error("Invalid parameters. Expected one parameter.");
				return -1;
			}

			var soupTargetDirectory = new Path(args[0]);
			var generateEngine = new BuildGenerateEngine();
			generateEngine.Generate(soupTargetDirectory);

			return 0;
		}
	}
}
