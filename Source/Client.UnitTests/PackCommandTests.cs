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
	public class PackCommandTests : IDisposable
	{
		LocalUserConfig _config;

		public PackCommandTests()
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
			var uut = new PackCommand();
			Assert.Equal("pack", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldPackAll()
		{
			var uut = new PackCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args, _config);
		}


	}
}
