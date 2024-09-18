// <copyright file="EventTypeFilter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal;

public class EventTypeFilter : IEventFilter
{
	private TraceEvents _types;

	public EventTypeFilter(TraceEvents eventTypes)
	{
		types = eventTypes;
	}

	public void Set(TraceEvents eventType)
	{
		types = eventType;
	}

	public void Disable(TraceEvents eventType)
	{
		types &= ~eventType;
	}

	public void Enable(TraceEvents eventType)
	{
		types |= eventType;
	}

	public bool IsEnabled(TraceEvents eventType)
	{
		return (types & eventType) == eventType;
	}

	public virtual bool ShouldTrace(TraceEvents eventType)
	{
		return IsEnabled(eventType);
	}
}