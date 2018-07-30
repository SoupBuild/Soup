// <copyright company="Soup" file="InitializeCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using Moq;
	using Soup.Api;
	using Xunit;

	/// <summary>
	/// Initialize Command Tests
	/// </summary>
	public class InitializeCommandTests : IDisposable
	{
		public InitializeCommandTests()
		{
			Singleton<ILogger>.Instance = new MockLogger();
			Singleton<LocalUserConfig>.Instance = new LocalUserConfig();
			Singleton<ISoupApi>.Instance = Mock.Of<ISoupApi>();
		}

		public void Dispose()
		{
			Singleton<LocalUserConfig>.Instance = null;
			Singleton<ISoupApi>.Instance = null;
		}

		[Fact]
		public void NameIsCorrect()
		{
			var uut = new InitializeCommand();
			Assert.Equal("init", uut.Name);
		}
	}
}
