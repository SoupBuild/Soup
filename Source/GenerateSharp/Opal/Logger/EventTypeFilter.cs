// <copyright file="EventTypeFilter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal;

public class EventTypeFilter : IEventFilter
{
	private TraceEvents _types;

	public EventTypeFilter(TraceEvents eventTypes)
	{
		_types = eventTypes;
	}

	public void Set(TraceEvents eventType)
	{
		_types = eventType;
	}

	public void Disable(TraceEvents eventType)
	{
		_types &= ~eventType;
	}

	public void Enable(TraceEvents eventType)
	{
		_types |= eventType;
	}

	public bool IsEnabled(TraceEvents eventType)
	{
		return (_types & eventType) == eventType;
	}

	public virtual bool ShouldTrace(TraceEvents eventType)
	{
		return IsEnabled(eventType);
	}
}