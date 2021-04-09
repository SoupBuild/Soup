// <copyright file="Log.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Opal
{
	/// <summary>
	/// The static logger class
	/// </summary>
	public class Log
	{
		/// <summary>
		/// Register the single event listener
		/// </summary>
		public static void RegisterListener(TraceListener? listener)
		{
			s_listener = listener;
		}

		/// <summary>
		/// Get access to the single event listener
		/// </summary>
		public static TraceListener EnsureListener()
		{
			if (s_listener == null)
				throw new InvalidOperationException("No Listener registered.");
			return s_listener;
		}

		/// <summary>
		/// Set the active ids to use for each event
		/// </summary>
		public static int GetActiveId()
		{
			return s_activeId;
		}

		public static void SetActiveId(int value)
		{
			s_activeId = value;
		}

		/// <summary>
		/// Log a high priority message
		/// </summary>
		public static void HighPriority(string message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag.HighPriority, id, message);
		}
		public static void HighPriority(string message)
		{
			HighPriority(message, s_activeId);
		}

		/// <summary>
		/// Log a generic infomational message
		/// </summary>
		public static void Info(string message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag.Information, id, message);
		}
		public static void Info(string message)
		{
			Info(message, s_activeId);
		}

		/// <summary>
		/// Log a diagnostic message
		/// </summary>
		public static void Diag(string message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag.Diagnostic, id, message);
		}
		public static void Diag(string message)
		{
			Diag(message, s_activeId);
		}


		/// <summary>
		/// Log a warning message
		/// </summary>
		public static void Warning(string message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag.Warning, id, message);
		}
		public static void Warning(string message)
		{
			Warning(message, s_activeId);
		}

		/// <summary>
		/// Log an error message
		/// </summary>
		public static void Error(string message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag.Error, id, message);
		}
		public static void Error(string message)
		{
			Error(message, s_activeId);
		}

		private static int s_activeId = 0;
		private static TraceListener? s_listener = null;
	}
}
