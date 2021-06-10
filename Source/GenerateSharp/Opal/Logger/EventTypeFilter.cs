// <copyright file="TraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
	public class EventTypeFilter : IEventFilter
	{
		public EventTypeFilter(TraceEventFlag eventTypes)
		{
			_types = eventTypes;
		}

		public void Set(TraceEventFlag eventType)
		{
			_types = eventType;
		}

		public void Disable(TraceEventFlag eventType)
		{
			_types &= ~eventType;
		}

		public void Enable(TraceEventFlag eventType)
		{
			_types |= eventType;
		}

		public bool IsEnabled(TraceEventFlag eventType)
		{
			return (_types & eventType) == eventType;
		}

		public virtual bool ShouldTrace(TraceEventFlag eventType)
		{
			return IsEnabled(eventType);
		}

		private TraceEventFlag _types;
	}
}
