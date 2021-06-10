// <copyright file="TestTraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal
{
	/// <summary>
	/// Test logger that wraps the base <see cref="TraceListener"/>
	/// </summary>
	public class TestTraceListener : TraceListener
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='TestTraceListener'/> class.
		/// </summary>
		public TestTraceListener() :
			base("", null, true, false)
		{
			_messages = new List<string>();
		}

		/// <summary>
		/// Get the messages
		/// </summary>
		public IReadOnlyList<string> GetMessages()
		{
			return _messages;
		}

		/// <summary>
		/// Writes a message and newline terminator
		/// </summary>
		protected override void WriteLine(string message)
		{
			_messages.Add(message);
		}

		private List<string> _messages;
	}
}
