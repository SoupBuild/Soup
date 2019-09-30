// <copyright file="BuildCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Build Command Tests
	/// </summary>
	class BuildCommandTests
	{
	public:
		[[Fact]]
		void NameIsCorrect()
		{
			auto uut = InitializeCommand();
		}
	};
}
