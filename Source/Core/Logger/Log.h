// <copyright file="Log.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "EventTypeFilter.h"

namespace Soup
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
        /// Log a generic infomational message
        /// </summary>
        static void Info(std::string_view message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Information, 0, message);
        }

        /// <summary>
        /// Log a verbose message
        /// </summary>
        static void Verbose(std::string_view message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Verbose, 0, message);
        }

        /// <summary>
        /// Log a trace message
        /// </summary>
        static void Trace(std::string_view message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Diagnostic, 0, message);
        }

        /// <summary>
        /// Log a warning message
        /// </summary>
        static void Warning(std::string_view message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Warning, 0, message);
        }

        /// <summary>
        /// Log an error message
        /// </summary>
        static void Error(std::string_view message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Error, 0, message);
        }

    private:
        /// <summary>
        /// Get access to the single event listener
        /// </summary>
        static TraceListener& EnsureListener()
        {
            if (s_listener == nullptr)
                throw std::runtime_error("No Listener registered.");
            return *s_listener;
        }

    private:
        static std::shared_ptr<TraceListener> s_listener;
    };

    std::shared_ptr<TraceListener> Log::s_listener = nullptr;
}
