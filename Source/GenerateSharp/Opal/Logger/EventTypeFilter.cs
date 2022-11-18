// <copyright file="EventTypeFilter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
	public class EventTypeFilter : IEventFilter
	{
		private TraceEventFlag types;

		public EventTypeFilter(TraceEventFlag eventTypes)
		{
			this.types = eventTypes;
		}

		public void Set(TraceEventFlag eventType)
		{
			this.types = eventType;
		}

		public void Disable(TraceEventFlag eventType)
		{
			this.types &= ~eventType;
		}

		public void Enable(TraceEventFlag eventType)
		{
			this.types |= eventType;
		}

		public bool IsEnabled(TraceEventFlag eventType)
		{
			return (this.types & eventType) == eventType;
		}

		public virtual bool ShouldTrace(TraceEventFlag eventType)
		{
			return this.IsEnabled(eventType);
		}
	}
}
