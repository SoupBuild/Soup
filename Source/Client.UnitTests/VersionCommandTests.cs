// <copyright company="Soup" file="VersionCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using System.Threading.Tasks;
	using Moq;
	using Soup.Api;
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
			var uut = new VersionCommand();
			Assert.Equal("version", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldVersionAll()
		{
			var uut = new VersionCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args);
		}
	}
}
