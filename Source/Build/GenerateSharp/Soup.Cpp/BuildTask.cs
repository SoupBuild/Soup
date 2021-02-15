

using System;
using System.Collections.Generic;

namespace Soup.Build
{
	public class BuildTask : IBuildTask
	{
		private IBuildState _factory;

		public BuildTask(IBuildState factory)
		{
			_factory = factory;
		}

		public void Execute()
		{
			Console.WriteLine("Do it!");

			var value = _factory.ActiveState["Test"];

			_factory.CreateOperation(
				"Title",
				"Exe.exe",
				"some arguments",
				"Workingdir",
				Array.Empty<string>(),
				Array.Empty<string>());
		}
	}
}
