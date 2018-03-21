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
	public class ViewCommandTests : IDisposable
	{
		LocalUserConfig _config;

		public ViewCommandTests()
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
			var uut = new ViewCommand();
			Assert.Equal("view", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldViewAll()
		{
			var uut = new ViewCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args, _config);
		}


	}
}
