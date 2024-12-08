// <copyright file="EventTypeFilter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal;

public class EventTypeFilter : IEventFilter
{
	private TraceEvents types;

	public EventTypeFilter(TraceEvents eventTypes)
	{
		this.types = eventTypes;
	}

	public void Set(TraceEvents eventType)
	{
		this.types = eventType;
	}

	public void Disable(TraceEvents eventType)
	{
		this.types &= ~eventType;
	}

	public void Enable(TraceEvents eventType)
	{
		this.types |= eventType;
	}

	public bool IsEnabled(TraceEvents eventType)
	{
		return (this.types & eventType) == eventType;
	}

	public virtual bool ShouldTrace(TraceEvents eventType)
	{
		return IsEnabled(eventType);
	}
}