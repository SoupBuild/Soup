// <copyright file="IEventFilter.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
    public enum TraceEventFlag
    {
        /// <summary>
        /// High Priority message.
        /// </summary>
        HighPriority = 1 << 0,

        /// <summary>
        /// Informational message.
        /// </summary>
        Information = 1 << 1,

        /// <summary>
        /// Diagnostic trace.
        /// </summary>
        Diagnostic = 1 << 2,

        /// <summary>
        /// Noncritical problem.
        /// </summary>
        Warning = 1 << 3,

        /// <summary>
        /// Recoverable error.
        /// </summary>
        Error = 1 << 4,

        /// <summary>
        /// Fatal error or application crash.
        /// </summary>
        Critical = 1 << 5,
    }

    public interface IEventFilter
    {
        bool ShouldTrace(TraceEventFlag eventType);
    }
}
