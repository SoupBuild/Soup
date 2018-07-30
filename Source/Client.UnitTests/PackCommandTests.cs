// <copyright company="Soup" file="PackCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using Moq;
	using Soup.Api;
	using Xunit;

	/// <summary>
	/// Pack Command Tests
	/// </summary>
	public class PackCommandTests : IDisposable
	{
		public PackCommandTests()
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
			var uut = new PackCommand();
			Assert.Equal("pack", uut.Name);
		}
	}
}
