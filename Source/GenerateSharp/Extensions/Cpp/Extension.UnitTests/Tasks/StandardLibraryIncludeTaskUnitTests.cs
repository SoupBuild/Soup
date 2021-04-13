// <copyright file="StandardLibraryIncludeTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Soup.Build.Cpp.UnitTests
{
	public class StandardLibraryIncludeTaskUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			var uut = new StandardLibraryIncludeTask();
		}

		[Fact]
		public void Execute()
		{
			var uut = new StandardLibraryIncludeTask();

			uut.Execute();
		}
	}
}
