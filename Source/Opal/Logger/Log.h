// <copyright file="Log.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "EventTypeFilter.h"

namespace Opal
{
	/// <summary>
	/// The static logger class
	/// </summary>
	export class Log
	{
	public:
		/// <summary>
		/// Register the single event listener
		/// </summary>
		static void RegisterListener(std::shared_ptr<TraceListener> listener)
		{
			s_listener = std::move(listener);
		}

		/// <summary>
		/// Get access to the single event listener
		/// </summary>
		static TraceListener& EnsureListener()
		{
			if (s_listener == nullptr)
				throw std::runtime_error("No Listener registered.");
			return *s_listener;
		}

		/// <summary>
		/// Set the active ids to use for each event
		/// </summary>
		static void SetActiveId(int value)
		{
			s_activeId = value;
		}

		/// <summary>
		/// Log a high priority message
		/// </summary>
		static void HighPriority(std::string_view message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag::HighPriority, id, message);
		}
		static void HighPriority(std::string_view message)
		{
			HighPriority(message, s_activeId);
		}

		/// <summary>
		/// Log a generic infomational message
		/// </summary>
		static void Info(std::string_view message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Information, id, message);
		}
		static void Info(std::string_view message)
		{
			Info(message, s_activeId);
		}

		/// <summary>
		/// Log a diagnostic message
		/// </summary>
		static void Diag(std::string_view message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Diagnostic, id, message);
		}
		static void Diag(std::string_view message)
		{
			Diag(message, s_activeId);
		}


		/// <summary>
		/// Log a warning message
		/// </summary>
		static void Warning(std::string_view message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Warning, id, message);
		}
		static void Warning(std::string_view message)
		{
			Warning(message, s_activeId);
		}

		/// <summary>
		/// Log an error message
		/// </summary>
		static void Error(std::string_view message, int id)
		{
			EnsureListener().TraceEvent(TraceEventFlag::Error, id, message);
		}
		static void Error(std::string_view message)
		{
			Error(message, s_activeId);
		}

	private:
		static int s_activeId;
		static std::shared_ptr<TraceListener> s_listener;
	};

	int Log::s_activeId = 0;
	std::shared_ptr<TraceListener> Log::s_listener = nullptr;
}
