// <copyright company="Soup" file="BuildCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using System.IO;
	using Soup.Api;
	using Soup.TestUtils;
	using Xunit;

	/// <summary>
	/// Build Command Tests
	/// </summary>
	public class BuildCommandTests : IDisposable
	{
		public BuildCommandTests()
		{
			var cwd = Directory.GetCurrentDirectory();
			Singleton<ILogger>.Instance = new MockLogger();
			Singleton<LocalUserConfig>.Instance = new LocalUserConfig();
			Singleton<ISoupApi>.Instance = new MockSoupApi();
		}

		public void Dispose()
		{
			Singleton<LocalUserConfig>.Instance = null;
			Singleton<ISoupApi>.Instance = null;
		}

		[Fact]
		public void NameIsCorrect()
		{
			var uut = new BuildCommand();
			Assert.Equal("build", uut.Name);
		}
	}
}
