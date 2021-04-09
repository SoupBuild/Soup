
using Opal;
using Soup.Build.Utilities;
using System.Threading.Tasks;

namespace Soup.Build.Generate
{
	class Program
	{
		static int Main(string[] args)
		{
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
