// <copyright file="TraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Globalization;
using System.Text;

namespace Opal;

/// <summary>
/// Base trace listener used to determine what events and properties to include in logs.
/// </summary>
public abstract class TraceListener
{
	private readonly IEventFilter? _filter;

	/// <summary>
	/// Initializes a new instance of the <see cref='TraceListener'/> class.
	/// </summary>
	protected TraceListener()
		: this(null, true, true)
	{
	}

	/// <summary>
	/// Initializes a new instance of the <see cref='TraceListener'/> class using the specified name as the
	/// listener.
	/// </summary>
	/// <param name="filter">The filter.</param>
	/// <param name="showEventType">A value indicating whether to show the event type.</param>
	/// <param name="showEventId">A value indicating whether to show the event id.</param>
	protected TraceListener(
		IEventFilter? filter,
		bool showEventType,
		bool showEventId)
	{
		_filter = filter;
		ShowEventType = showEventType;
		ShowEventId = showEventId;
	}

	/// <summary>
	/// Gets a value indicating whether there is a custom event filter.this.
	/// </summary>
	public bool HasFilter()
	{
		return _filter != null;
	}

	/// <summary>
	/// Gets or sets a value indicating whether to show or hide the event type.
	/// </summary>
	public bool ShowEventType { get; }

	/// <summary>
	/// Gets or sets a value indicating whether to show or hide the event id.
	/// </summary>
	public bool ShowEventId { get; set; }

	/// <summary>
	/// All other TraceEvent methods come through this one.
	/// </summary>
	/// <param name="eventType">The event type.</param>
	/// <param name="id">The event id.</param>
	/// <param name="message">The event message.</param>
	public void TraceEvent(
		TraceEvents eventType,
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
		_ = builder.Append(message);

		bool isEmpty = builder.Length == 0;
		if (isEmpty)
			WriteLine(string.Empty);
		else
			WriteLine(builder.ToString());
	}

	/// <summary>
	/// Implementation dependent write methods.
	/// </summary>
	/// <param name="message">The message.</param>
	protected abstract void WriteLine(string message);

	///// <summary>
	///// Trace Event.
	///// </summary>
	//// void TraceEvent(
	////	 string source,
	////	 TraceEvents eventType,
	////	 int id,
	////	 string format,
	////	 params object[] args)
	//// {
	////	 if (Filter != null && !Filter.ShouldTrace( source, eventType, id, format, args, null, null))
	////	 {
	////		 return;
	////	 }

	////	 WriteHeader(source, eventType, id);
	////	 if (args != null)
	////	 {
	////		 WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
	////	 }
	////	 else
	////	 {
	////		 WriteLine(format);
	////	 }
	//// }

	/// <summary>
	/// Write the header to the target listener.
	/// </summary>
	private void WriteHeader(
		StringBuilder builder,
		TraceEvents eventType,
		int id)
	{
		if (ShowEventType)
		{
			switch (eventType)
			{
				case TraceEvents.HighPriority:
					_ = builder.Append("HIGH");
					break;
				case TraceEvents.Information:
					_ = builder.Append("INFO");
					break;
				case TraceEvents.Diagnostic:
					_ = builder.Append("DIAG");
					break;
				case TraceEvents.Warning:
					_ = builder.Append("WARN");
					break;
				case TraceEvents.Error:
					_ = builder.Append("ERRO");
					break;
				case TraceEvents.Critical:
					_ = builder.Append("CRIT");
					break;
				case TraceEvents.None:
					break;
				default:
					_ = builder.Append("UNKN");
					break;
			}

			_ = builder.Append(": ");
		}

		if (ShowEventId)
		{
			_ = builder.Append(CultureInfo.InvariantCulture, $"{id}>");
		}
	}
}