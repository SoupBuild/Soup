// <copyright file="Log.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    using System.Diagnostics;

    /// <summary>
    /// The static logger class
    /// </summary>
    public static class Log
    {
        private static TraceSource _source;

        static Log()
        {
            // Setup the source
            _source = new TraceSource("Log", SourceLevels.All);

            // Register the default console listener
            _source.Listeners.Add(
                new ConsoleTraceListener()
                {
                    Filter = new EventTypeFilter(SourceLevels.Information | SourceLevels.Error | SourceLevels.Warning),
                    ShowSourceName = false,
                    ShowEventId = false,
                    ShowEventType = false,
                });
        }

        /// <summary>
        /// Log an informational message
        /// </summary>
        public static void Info(string message)
        {
            _source.TraceEvent(TraceEventType.Information, 0, message);
        }

        /// <summary>
        /// Log a verbose message
        /// </summary>
        public static void Verbose(string message)
        {
            _source.TraceEvent(TraceEventType.Verbose, 0, message);
        }

        /// <summary>
        /// Log a warning message
        /// </summary>
        public static void Warning(string message)
        {
            _source.TraceEvent(TraceEventType.Warning, 0, message);
        }

        /// <summary>
        /// Log an error message
        /// </summary>
        public static void Error(string message)
        {
            _source.TraceEvent(TraceEventType.Error, 0, message);
        }
    }
}
