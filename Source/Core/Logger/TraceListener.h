// <copyright file="TraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    enum class TraceEventFlag : uint32_t
    {
        // Informational message.
        Information = 1 << 0,
        // Debugging trace.
        Verbose = 1 << 1,
        // Diagnostic trace.
        Diagnostic = 1 << 2,
        // Noncritical problem.
        Warning = 1 << 3,
        // Recoverable error.
        Error = 1 << 4,
        // Fatal error or application crash.
        Critical = 1 << 5,
    };

    class IEventFilter
    {
    public:
        virtual bool ShouldTrace(TraceEventFlag eventType) = 0;
    };

    /// <summary>
    /// Base trace listener used to determine what events and properties to include in logs
    /// </summary>
    class TraceListener
    {
    protected:
        /// <summary>
        /// Initializes a new instance of the <see cref='TraceListener'/> class.
        /// </summary>
        TraceListener() :
            TraceListener("", nullptr, true, true)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref='TraceListener'/> class using the specified name as the
        /// listener.
        /// </summary>
        TraceListener(
            std::string name,
            std::shared_ptr<IEventFilter> filter,
            bool showEventType,
            bool showEventId) :
            _name(std::move(name)),
            _filter(std::move(filter)),
            _showEventType(showEventType),
            _showEventId(showEventId)
        {
        }

        /// <summary>
        /// Implementation dependant write methods
        /// </summary>
        virtual void Write(const std::string& message) = 0;
        virtual void WriteLine(const std::string& message) = 0;

    public:
        /// <summary>
        /// Gets a value indicating whether there is a custom event filter
        /// </summary>
        bool HasFilter()
        {
            return _filter != nullptr;
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show or hide the event type
        /// </summary>
        bool GetShowEventType()
        {
            return _showEventType;
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show or hide the event id
        /// </summary>
        bool GetShowEventId()
        {
            return _showEventId;
        }

        /// <summary>
        /// All other TraceEvent methods come through this one.
        /// </summary>
        void TraceEvent(
            TraceEventFlag eventType,
            int id,
            const std::string& message)
        {
            if (HasFilter() && ! _filter->ShouldTrace(eventType))
            {
                return;
            }

            WriteHeader(eventType, id);
            WriteLine(message);
        }

        /// <summary>
        /// Trace Event
        /// </summary>
        // void TraceEvent(
        //     string source,
        //     TraceEventFlag eventType,
        //     int id,
        //     string format,
        //     params object[] args)
        // {
        //     if (Filter != null && !Filter.ShouldTrace( source, eventType, id, format, args, null, null))
        //     {
        //         return;
        //     }

        //     WriteHeader(source, eventType, id);
        //     if (args != null)
        //     {
        //         WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
        //     }
        //     else
        //     {
        //         WriteLine(format);
        //     }
        // }

    private:
        /// <summary>
        /// Write the header to the target listener
        /// </summary>
        void WriteHeader(
            TraceEventFlag eventType,
            int id)
        {
            std::stringstream builder;

            if (GetShowEventType())
            {
                switch (eventType)
                {
                    case TraceEventFlag::Information:
                        builder << "INFO";
                        break;
                    case TraceEventFlag::Verbose:
                        builder << "VERB";
                        break;
                    case TraceEventFlag::Diagnostic:
                        builder << "DIAG";
                        break;
                    case TraceEventFlag::Warning:
                        builder << "WARN";
                        break;
                    case TraceEventFlag::Error:
                        builder << "ERRO";
                        break;
                    case TraceEventFlag::Critical: 
                        builder << "CRIT";
                        break;
                    default:
                        builder << "UNKN";
                        break;
                }

                builder << ": ";
            }

            if (GetShowEventId())
            {
                builder << std::to_string(id) << " : ";
            }

            auto header = builder.str();
            if (!header.empty())
            {
                Write(header);
            }
        }

    private:
        std::string _name;
        std::shared_ptr<IEventFilter> _filter;
        bool _showEventType;
        bool _showEventId;
    };
}
