// <copyright file="PublishCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Publish Command Tests
	/// </summary>
	class PublishCommandTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = PackCommand();
		}
	};
}
