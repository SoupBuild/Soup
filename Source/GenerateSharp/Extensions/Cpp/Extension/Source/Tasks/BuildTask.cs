

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
			_factory.LogTrace(TraceLevel.Debug, "Do it!");

			_factory.CreateOperation(
				"Title",
				"Exe.exe",
				"some arguments",
				"C:/Workingdir",
				Array.Empty<string>(),
				Array.Empty<string>());
		}
	}
}
