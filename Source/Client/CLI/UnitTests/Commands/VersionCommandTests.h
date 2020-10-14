// <copyright file="VersionCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
	/// <summary>
	/// Version Command Tests
	/// </summary>
	class VersionCommandTests
	{
	public:
		[[Fact]]
		void Initialize()
		{
			auto uut = VersionCommand();
		}
	};
}
