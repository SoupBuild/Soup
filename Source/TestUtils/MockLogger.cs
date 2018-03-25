// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup
{
	public class MockLogger : ILogger
	{
		public IList<string> MessageList { get; private set; }
		public IList<string> VerboseList { get; private set; }
		public IList<string> WarningList { get; private set; }
		public IList<string> ErrorList { get; private set; }

		public MockLogger()
		{
			MessageList = new List<string>();
			VerboseList = new List<string>();
			WarningList = new List<string>();
			ErrorList = new List<string>();
		}

		public void Message(string message)
		{
			MessageList.Add(message);
		}

		public void Message(string message, ConsoleColor color)
		{
			MessageList.Add(message);
		}

		public void Verbose(string message)
		{
			VerboseList.Add(message);
		}

		public void Warning(string message)
		{
			WarningList.Add(message);
		}

		public void Error(string message)
		{
			ErrorList.Add(message);
		}
	}
}
