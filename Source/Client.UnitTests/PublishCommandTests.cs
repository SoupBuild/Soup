// <copyright company="Soup" file="PublishCommandTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
	using System;
	using Moq;
	using Soup.Api;
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
			Singleton<ISoupApi>.Instance = Mock.Of<ISoupApi>();
		}

		public void Dispose()
		{
			Singleton<LocalUserConfig>.Instance = null;
			Singleton<ISoupApi>.Instance = null;
		}
	}
}
