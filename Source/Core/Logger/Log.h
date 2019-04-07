// <copyright file="Log.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ConsoleTraceListener.h"
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
        /// Gets or sets the value indicating whether diagnostic logging is enabled or not
        /// </summary>
        static void SetDiagnosticEnabled(bool value)
        {
            if (value)
            {
                EnsureFilter()->Enable(TraceEventFlag::Diagnostic);
            }
            else
            {
                EnsureFilter()->Disable(TraceEventFlag::Diagnostic);
            }
        }

        /// <summary>
        /// Gets or sets the value indicating whether verbose logging is enabled or not
        /// </summary>
        static void SetVerboseEnabled(bool value)
        {
            if (value)
            {
                EnsureFilter()->Enable(TraceEventFlag::Verbose);
            }
            else
            {
                EnsureFilter()->Disable(TraceEventFlag::Verbose);
            }
        }

        /// <summary>
        /// Log a generic infomational message
        /// </summary>
        static void Info(const std::string& message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Information, 0, message);
        }

        /// <summary>
        /// Log a verbose message
        /// </summary>
        static void Verbose(const std::string& message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Verbose, 0, message);
        }

        /// <summary>
        /// Log a trace message
        /// </summary>
        static void Trace(const std::string& message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Diagnostic, 0, message);
        }

        /// <summary>
        /// Log a warning message
        /// </summary>
        static void Warning(const std::string& message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Warning, 0, message);
        }

        /// <summary>
        /// Log an error message
        /// </summary>
        static void Error(const std::string& message)
        {
            EnsureListener().TraceEvent(TraceEventFlag::Error, 0, message);
        }

    private:
        /// <summary>
        /// Get access to the single event listener
        /// </summary>
        static TraceListener& EnsureListener()
        {
            // Setup the listener
            static std::shared_ptr<TraceListener> listener =
                std::make_shared<ConsoleTraceListener>(
                    "Log",
                    EnsureFilter(),
                    false,
                    false);

            return *listener;
        }

        /// <summary>
        /// Get access to the single event filter
        /// </summary>
        static const std::shared_ptr<EventTypeFilter>& EnsureFilter()
        {
            // Setup the filter
            auto defaultTypes = 
                    static_cast<uint32_t>(TraceEventFlag::Information) |
                    static_cast<uint32_t>(TraceEventFlag::Warning) |
                    static_cast<uint32_t>(TraceEventFlag::Error) |
                    static_cast<uint32_t>(TraceEventFlag::Critical);
            static std::shared_ptr<EventTypeFilter> filter = 
                std::make_shared<EventTypeFilter>(
                    static_cast<TraceEventFlag>(defaultTypes));

            return filter;
        }
    };
}
