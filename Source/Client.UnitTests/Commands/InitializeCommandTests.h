// <copyright file="InitializeCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Initialize Command Tests
	/// </summary>
	class InitializeCommandTests
	{
	public:
		[[Fact]]
		void NameIsCorrect()
		{
			auto uut = InitializeCommand();
		}
	};
}
