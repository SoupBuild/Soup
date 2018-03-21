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
	public class GenerateCommandTests : IDisposable
	{
		LocalUserConfig _config;

		public GenerateCommandTests()
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
			var uut = new GenerateCommand();
			Assert.Equal("generate", uut.Name);
		}

		[Fact]
		public async Task CallWithZeroArgsShouldGenerateAll()
		{
			var uut = new GenerateCommand();

			var args = new string[] { };
			await uut.InvokeAsync(args, _config);
		}


	}
}
