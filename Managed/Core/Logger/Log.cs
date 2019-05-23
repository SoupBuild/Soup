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
        private static EventTypeFilter _consoleEventFilter;

        static Log()
        {
            // Setup the source
            _source = new TraceSource("Log", SourceLevels.All);

            // Register the default console listener
            _consoleEventFilter = new EventTypeFilter(SourceLevels.Information | SourceLevels.Error | SourceLevels.Warning);
            _source.Listeners.Add(
                new ConsoleTraceListener()
                {
                    Filter = _consoleEventFilter,
                    ShowSourceName = false,
                    ShowEventId = false,
                    ShowEventType = false,
                });
        }

        /// <summary>
        /// Gets or sets the value indicating whether verbose logging is enabled or not
        /// </summary>
        public static bool EnableVerbose
        {
            get
            {
                return _consoleEventFilter.EventType.HasFlag(SourceLevels.Verbose);
            }

            set
            {
                if (EnableVerbose != value)
                {
                    if (value)
                    {
                        _consoleEventFilter.EventType |= SourceLevels.Verbose;
                    }
                    else
                    {
                        _consoleEventFilter.EventType &= ~SourceLevels.Verbose;
                    }
                }
            }
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
