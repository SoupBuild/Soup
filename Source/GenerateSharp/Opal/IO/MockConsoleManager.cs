// <copyright file="MockConsoleManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.IO
{
	using global::System.Collections.Generic;

	/// <summary>
	/// The mock console manager
	/// TODO: Move into test project.
	/// </summary>
	public class MockConsoleManager : IConsoleManager
	{
		private List<string> _requests;
		private MockConsoleInputStream _standardInput;

		/// <summary>
		/// Initializes a new instance of the <see cref='MockConsoleManager'/> class.
		/// </summary>
		public MockConsoleManager()
		{
			this._requests = new List<string>();
			this._standardInput = new MockConsoleInputStream(this._requests);
		}

		/// <summary>
		/// Get the requests.
		/// </summary>
		public IReadOnlyList<string> GetRequests()
		{
			return this._requests;
		}

		/// <summary>
		/// Get the mock input stream.
		/// </summary>
		public MockConsoleInputStream GetMockInputStream()
		{
			return this._standardInput;
		}

		/// <summary>
		/// Get the standard input stream.
		/// </summary>
		public IConsoleInputStream GetStandardInput()
		{
			this._requests.Add("GetStandardInput");

			return this._standardInput;
		}
	}
}
