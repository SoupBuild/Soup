// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using Soup.Api;
using Soup.TestUtils;
using System;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Client.UnitTests
{
	public class BuildCommandTests : IDisposable
	{
		LocalUserConfig _config;

		public BuildCommandTests()
		{
			_config = new LocalUserConfig();
			Singleton<ISoupApi>.Instance = new MockSoupApi();
		}

		public void Dispose()
		{
			_config = null;
		}

		[Fact]
		public void NameIsCorrect()
		{
			var uut = new BuildCommand();
			Assert.Equal("build", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldInstallAll()
		{
			var uut = new BuildCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args, _config);
		}


	}
}
