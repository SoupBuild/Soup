// <copyright file="TraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Text;

namespace Opal
{
	public enum TraceEventFlag
	{
		// High Priority message.
		HighPriority = 1 << 0,
		// Informational message.
		Information = 1 << 1,
		// Diagnostic trace.
		Diagnostic = 1 << 2,
		// Noncritical problem.
		Warning = 1 << 3,
		// Recoverable error.
		Error = 1 << 4,
		// Fatal error or application crash.
		Critical = 1 << 5,
	}

	public interface IEventFilter
	{
		bool ShouldTrace(TraceEventFlag eventType);
	};

	/// <summary>
	/// Base trace listener used to determine what events and properties to include in logs
	/// </summary>
	public abstract class TraceListener
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='TraceListener'/> class.
		/// </summary>
		protected TraceListener() :
			this("", null, true, true)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='TraceListener'/> class using the specified name as the
		/// listener.
		/// </summary>
		protected TraceListener(
			string name,
			IEventFilter? filter,
			bool showEventType,
			bool showEventId)
		{
			_name = name;
			_filter = filter;
			_showEventType = showEventType;
			_showEventId = showEventId;
		}

		/// <summary>
		/// Implementation dependant write methods
		/// </summary>
		protected abstract void WriteLine(string message);

		/// <summary>
		/// Gets a value indicating whether there is a custom event filter
		/// </summary>
		public bool HasFilter()
		{
			return _filter != null;
		}

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event type
		/// </summary>
		public bool GetShowEventType()
		{
			return _showEventType;
		}

		/// <summary>
		/// Gets or sets a value indicating whether to show or hide the event id
		/// </summary>
		public bool GetShowEventId()
		{
			return _showEventId;
		}
		public void SetShowEventId(bool value)
		{
			_showEventId = value;
		}

		/// <summary>
		/// All other TraceEvent methods come through this one.
		/// </summary>
		public void TraceEvent(
			TraceEventFlag eventType,
			int id,
			string message)
		{
			if (_filter != null && !_filter.ShouldTrace(eventType))
			{
				return;
			}

			// Build up the resulting message with required header/footer
			var builder = new StringBuilder();
			WriteHeader(builder, eventType, id);
			builder.Append(message);

			bool isEmpty = builder.Length == 0;
			if (isEmpty)
				WriteLine("");
			else
				WriteLine(builder.ToString());
		}

		/// <summary>
		/// Trace Event
		/// </summary>
		// void TraceEvent(
		//	 string source,
		//	 TraceEventFlag eventType,
		//	 int id,
		//	 string format,
		//	 params object[] args)
		// {
		//	 if (Filter != null && !Filter.ShouldTrace( source, eventType, id, format, args, null, null))
		//	 {
		//		 return;
		//	 }

		//	 WriteHeader(source, eventType, id);
		//	 if (args != null)
		//	 {
		//		 WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
		//	 }
		//	 else
		//	 {
		//		 WriteLine(format);
		//	 }
		// }

		/// <summary>
		/// Write the header to the target listener
		/// </summary>
		private void WriteHeader(
			StringBuilder builder,
			TraceEventFlag eventType,
			int id)
		{
			if (GetShowEventType())
			{
				switch (eventType)
				{
					case TraceEventFlag.HighPriority:
						builder.Append("HIGH");
						break;
					case TraceEventFlag.Information:
						builder.Append("INFO");
						break;
					case TraceEventFlag.Diagnostic:
						builder.Append("DIAG");
						break;
					case TraceEventFlag.Warning:
						builder.Append("WARN");
						break;
					case TraceEventFlag.Error:
						builder.Append("ERRO");
						break;
					case TraceEventFlag.Critical: 
						builder.Append("CRIT");
						break;
					default:
						builder.Append("UNKN");
						break;
				}

				builder.Append(": ");
			}

			if (GetShowEventId())
			{
				builder.Append($"{id}>");
			}
		}

		private string _name;
		private IEventFilter? _filter;
		private bool _showEventType;
		private bool _showEventId;
	}
}
