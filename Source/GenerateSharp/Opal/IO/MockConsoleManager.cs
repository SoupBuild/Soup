// <copyright file="MockConsoleManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.IO
{
	/// <summary>
	/// The mock console manager
	/// TODO: Move into test project
	/// </summary>
	public class MockConsoleManager : IConsoleManager
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='MockConsoleManager'/> class.
		/// </summary>
		public MockConsoleManager()
		{
			_requests = new List<string>();
			_standardInput = new MockConsoleInputStream(_requests);
		}

		/// <summary>
		/// Get the requests
		/// </summary>
		IReadOnlyList<string> GetRequests()
		{
			return _requests;
		}

		/// <summary>
		/// Get the mock input stream
		/// </summary>
		MockConsoleInputStream GetMockInputStream()
		{
			return _standardInput;
		}

		/// <summary>
		/// Get the standard input stream
		/// </summary>
		public IConsoleInputStream GetStandardInput()
		{
			_requests.Add("GetStandardInput");

			return _standardInput;
		}

        private List<string> _requests;
		private MockConsoleInputStream _standardInput;
	}
}
