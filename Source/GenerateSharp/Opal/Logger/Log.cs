// <copyright file="Log.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal;

/// <summary>
/// The static logger class.
/// </summary>
public static class Log
{
	private static TraceListener? listener;

	/// <summary>
	/// Register the single event listener.
	/// </summary>
	/// <param name="listener">The listener.</param>
	public static void RegisterListener(TraceListener? listener)
	{
		Log.listener = listener;
	}

	/// <summary>
	/// Get access to the single event listener.
	/// </summary>
	public static TraceListener EnsureListener()
	{
		return listener ?? throw new InvalidOperationException("No Listener registered.");
	}

	/// <summary>
	/// Set the active ids to use for each event.
	/// </summary>
	public static int ActiveId { get; set; }

	/// <summary>
	/// Log a high priority message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="id">The message id.</param>
	public static void HighPriority(string message, int id)
	{
		EnsureListener().TraceEvent(TraceEvents.HighPriority, id, message);
	}

	public static void HighPriority(string message)
	{
		HighPriority(message, ActiveId);
	}

	/// <summary>
	/// Log a generic informational message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="id">The message id.</param>
	public static void Info(string message, int id)
	{
		EnsureListener().TraceEvent(TraceEvents.Information, id, message);
	}

	public static void Info(string message)
	{
		Info(message, ActiveId);
	}

	/// <summary>
	/// Log a diagnostic message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="id">The message id.</param>
	public static void Diag(string message, int id)
	{
		EnsureListener().TraceEvent(TraceEvents.Diagnostic, id, message);
	}

	public static void Diag(string message)
	{
		Diag(message, ActiveId);
	}

	/// <summary>
	/// Log a warning message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="id">The message id.</param>
	public static void Warning(string message, int id)
	{
		EnsureListener().TraceEvent(TraceEvents.Warning, id, message);
	}

	public static void Warning(string message)
	{
		Warning(message, ActiveId);
	}

	/// <summary>
	/// Log an error message.
	/// </summary>
	/// <param name="message">The message.</param>
	/// <param name="id">The message id.</param>
	public static void Error(string message, int id)
	{
		EnsureListener().TraceEvent(TraceEvents.Error, id, message);
	}

	public static void Error(string message)
	{
		Error(message, ActiveId);
	}
}
