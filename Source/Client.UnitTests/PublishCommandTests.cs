// <copyright company="Soup" file="PublishCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using Soup.Api;
	using Soup.TestUtils;
	using Xunit;

	/// <summary>
	/// Publish Command Tests
	/// </summary>
	public class PublishCommandTests : IDisposable
	{
		public PublishCommandTests()
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
			var uut = new PublishCommand();
			Assert.Equal("publish", uut.Name);
		}
	}
}
