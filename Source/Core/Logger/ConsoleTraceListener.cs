// <copyright company="Soup" file="ConsoleTraceListener.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>


namespace Soup
{
	using System;
	using System.Diagnostics;

	/// <summary>
	/// Console logger that wraps the base <see cref="ExtendedTraceListener"/>
	/// </summary>
	public class ConsoleTraceListener : ExtendedTraceListener
	{
		private TraceEventType _currentEvent;

		/// <summary>
		/// Initializes a new instance of the <see cref='ConsoleTraceListener'/> class.</para>
		/// </summary>
		public ConsoleTraceListener() :
			base()
		{
		}

		/// <summary>
		/// Trace Data
		/// </summary>
		public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, object data)
		{
			_currentEvent = eventType;
			base.TraceData(eventCache, source, eventType, id, data);
		}

		/// <summary>
		/// Trace Data
		/// </summary>
		public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, params object[] data)
		{
			_currentEvent = eventType;
			base.TraceData(eventCache, source, eventType, id, data);
		}

		/// <summary>
		/// All other TraceEvent methods come through this one.
		/// </summary>
		public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string message)
		{
			_currentEvent = eventType;
			base.TraceEvent(eventCache, source, eventType, id, message);
		}

		/// <summary>
		/// Trace Event
		/// </summary>
		public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string format, params object[] args)
		{
			_currentEvent = eventType;
			base.TraceEvent(eventCache, source, eventType, id, format, args);
		}

		/// <summary>
		/// Writes a message
		/// </summary>
		public override void Write(string message)
		{
			if (NeedIndent)
				WriteIndent();

			try
			{
				var previousColor = Console.ForegroundColor;
				SetConsoleColor();
				Console.Write(message);
				Console.ForegroundColor = previousColor;
			}
			catch (ObjectDisposedException)
			{
			}
		}

		/// <summary>
		/// Writes a message 
		/// to this instance's <see cref='TextWriterExtendedTraceListener.Writer'/> followed by a line terminator. The
		/// default line terminator is a carriage return followed by a line feed (\r\n).
		/// </summary>
		public override void WriteLine(string message)
		{
			if (NeedIndent)
				WriteIndent();

			try
			{
				var previousColor = Console.ForegroundColor;
				SetConsoleColor();
				Console.WriteLine(message);
				Console.ForegroundColor = previousColor;

				NeedIndent = true;
			}
			catch (ObjectDisposedException)
			{
			}
		}

		private void SetConsoleColor()
		{
			switch (_currentEvent)
			{
				case TraceEventType.Error:
					Console.ForegroundColor = ConsoleColor.Red;
					break;
				case TraceEventType.Warning:
					Console.ForegroundColor = ConsoleColor.Yellow;
					break;
			}
		}
	}
}
