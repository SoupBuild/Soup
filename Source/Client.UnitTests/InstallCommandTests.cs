// <copyright company="Soup" file="InstallCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using Soup.Api;
	using Soup.TestUtils;
	using Xunit;

	/// <summary>
	/// Install Command Tests
	/// </summary>
	public class InstallCommandTests : IDisposable
	{
		public InstallCommandTests()
		{
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
			var uut = new InstallCommand();
			Assert.Equal("install", uut.Name);
		}
	}
}
