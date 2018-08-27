// <copyright company="Soup" file="ExtendedTraceListener.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>


namespace Soup
{
	using System;
	using System.Text;
	using System.Globalization;
	using System.Collections;
	using System.Diagnostics;

	/// <summary>
	/// Extends the base <see cref="TraceListener"/> to add extra setting properties
	/// </summary>
	public abstract class ExtendedTraceListener : TraceListener
	{
		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the source name
		/// </summary>
		public bool ShowSourceName { get; set; } = true;

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event type
		/// </summary>
		public bool ShowEventType { get; set; } = true;

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event id
		/// </summary>
		public bool ShowEventId { get; set; } = true;

		/// <summary>
		/// Initializes a new instance of the <see cref='ExtendedTraceListener'/> class.</para>
		/// </summary>
		protected ExtendedTraceListener() :
			base()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='ExtendedTraceListener'/> class using the specified name as the
		/// listener.
		/// </summary>
		protected ExtendedTraceListener(string name) :
			base(name)
		{
		}

		/// <summary>
		/// Trace Data
		/// </summary>
		public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, object data)
		{
			if (Filter != null && !Filter.ShouldTrace(eventCache, source, eventType, id, null, null, data, null))
				return;

			WriteHeader(source, eventType, id);
			string datastring = string.Empty;
			if (data != null)
				datastring = data.ToString();

			WriteLine(datastring);
			WriteFooter(eventCache);
		}

		/// <summary>
		/// Trace Data
		/// </summary>
		public override void TraceData(TraceEventCache eventCache, string source, TraceEventType eventType, int id, params object[] data)
		{
			if (Filter != null && !Filter.ShouldTrace(eventCache, source, eventType, id, null, null, null, data))
				return;

			WriteHeader(source, eventType, id);

			StringBuilder sb = new StringBuilder();
			if (data != null)
			{
				for (int i = 0; i < data.Length; i++)
				{
					if (i != 0)
						sb.Append(", ");

					if (data[i] != null)
						sb.Append(data[i].ToString());
				}
			}
			WriteLine(sb.ToString());

			WriteFooter(eventCache);
		}

		/// <summary>
		/// All other TraceEvent methods come through this one.
		/// </summary>
		public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string message)
		{
			if (Filter != null && !Filter.ShouldTrace(eventCache, source, eventType, id, message, null, null, null))
				return;

			WriteHeader(source, eventType, id);
			WriteLine(message);

			WriteFooter(eventCache);
		}

		/// <summary>
		/// Trace Event
		/// </summary>
		public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType eventType, int id, string format, params object[] args)
		{
			if (Filter != null && !Filter.ShouldTrace(eventCache, source, eventType, id, format, args, null, null))
				return;

			WriteHeader(source, eventType, id);
			if (args != null)
				WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
			else
				WriteLine(format);

			WriteFooter(eventCache);
		}

		private void WriteHeader(string source, TraceEventType eventType, int id)
		{
			StringBuilder builder = new StringBuilder();

			if (ShowSourceName)
			{
				builder.Append(source);
				builder.Append(" ");
			}

			if (ShowEventType)
			{
				builder.Append(eventType.ToString());
				builder.Append(": ");
			}

			if (ShowEventId)
			{
				builder.Append(id.ToString(CultureInfo.InvariantCulture));
				builder.Append(" : ");
			}

			Write(builder.ToString());
		}

		private void WriteFooter(TraceEventCache eventCache)
		{
			if (eventCache == null)
				return;

			IndentLevel++;
			if (IsEnabled(TraceOptions.ProcessId))
				WriteLine("ProcessId=" + eventCache.ProcessId);

			if (IsEnabled(TraceOptions.LogicalOperationStack))
			{
				Write("LogicalOperationStack=");
				Stack operationStack = eventCache.LogicalOperationStack;
				bool first = true;
				foreach (object obj in operationStack)
				{
					if (!first)
						Write(", ");
					else
						first = false;

					Write(obj.ToString());
				}

				WriteLine(string.Empty);
			}

			if (IsEnabled(TraceOptions.ThreadId))
				WriteLine("ThreadId=" + eventCache.ThreadId);

			if (IsEnabled(TraceOptions.DateTime))
				WriteLine("DateTime=" + eventCache.DateTime.ToString("o", CultureInfo.InvariantCulture));

			if (IsEnabled(TraceOptions.Timestamp))
				WriteLine("Timestamp=" + eventCache.Timestamp);

			if (IsEnabled(TraceOptions.Callstack))
				WriteLine("Callstack=" + eventCache.Callstack);
			IndentLevel--;
		}

		internal bool IsEnabled(TraceOptions opts)
		{
			return (opts & TraceOutputOptions) != 0;
		}
	}
}
