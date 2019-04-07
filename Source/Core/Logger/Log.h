// <copyright file="Log.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    export enum class LogLevel
    {
        Normal,
        Verbose,
        Diagnostic,
    };

    /// <summary>
    /// The static logger class
    /// </summary>
    export class Log
    {
    public:
        // static Log()
        // {
            // // Setup the source
            // _source = new TraceSource("Log", SourceLevels.All);

            // // Register the default console listener
            // _consoleEventFilter = new EventTypeFilter(SourceLevels.Information | SourceLevels.Error | SourceLevels.Warning);
            // _source.Listeners.Add(
            //     new ConsoleTraceListener()
            //     {
            //         Filter = _consoleEventFilter,
            //         ShowSourceName = false,
            //         ShowEventId = false,
            //         ShowEventType = false,
            //     });
        //}

        /// <summary>
        /// Gets or sets the value indicating whether verbose logging is enabled or not
        /// </summary>
        static LogLevel GetLevel()
        {
            return LogLevel::Normal;
        }

        /// <summary>
        /// Log a message
        /// </summary>
        static void Message(const std::string& message)
        {
            // _source.TraceEvent(TraceEventType.Information, 0, message);
            std::cout << message << std::endl;
        }

        /// <summary>
        /// Log a verbose message
        /// </summary>
        static void Verbose(const std::string& message)
        {
            // _source.TraceEvent(TraceEventType.Verbose, 0, message);
            std::cout << message << std::endl;
        }

        /// <summary>
        /// Log a trace message
        /// </summary>
        static void Trace(const std::string& message)
        {
            // _source.TraceEvent(TraceEventType.Error, 0, message);
            std::cout << message << std::endl;
        }

        /// <summary>
        /// Log a warning message
        /// </summary>
        static void Warning(const std::string& message)
        {
            // _source.TraceEvent(TraceEventType.Warning, 0, message);
            std::cout << message << std::endl;
        }

        /// <summary>
        /// Log an error message
        /// </summary>
        static void Error(const std::string& message)
        {
            // _source.TraceEvent(TraceEventType.Error, 0, message);
            std::cout << message << std::endl;
        }

    private:
        // private static TraceSource _source;
        // private static EventTypeFilter _consoleEventFilter;
    };
}
