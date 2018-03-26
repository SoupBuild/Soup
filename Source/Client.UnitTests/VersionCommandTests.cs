// <copyright company="Soup" file="VersionCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using System.Collections.Generic;
	using System.Threading.Tasks;
	using Soup.Api;
	using Soup.TestUtils;
	using Xunit;

	/// <summary>
	/// Version Command Tests
	/// </summary>
	public class VersionCommandTests : IDisposable
	{
		private MockLogger _logger;

		public VersionCommandTests()
		{
			Singleton<ILogger>.Instance = _logger = new MockLogger();
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
			var uut = new VersionCommand();
			Assert.Equal("version", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldVersionAll()
		{
			var uut = new VersionCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args);

			Assert.Equal(new List<string> { "0.1.1" }, _logger.MessageList);
		}
	}
}
