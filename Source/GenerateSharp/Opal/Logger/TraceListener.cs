// <copyright file="TraceListener.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal
{
    using global::System.Text;

    /// <summary>
    /// Base trace listener used to determine what events and properties to include in logs.
    /// </summary>
    public abstract class TraceListener
    {
        private string name;
        private IEventFilter? filter;
        private bool showEventType;
        private bool showEventId;

        /// <summary>
        /// Initializes a new instance of the <see cref='TraceListener'/> class.
        /// </summary>
        protected TraceListener()
            : this(string.Empty, null, true, true)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref='TraceListener'/> class using the specified name as the
        /// listener.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="filter">The filter.</param>
        /// <param name="showEventType">A value indicating whether to show the event type.</param>
        /// <param name="showEventId">A value indicating whether to show the event id.</param>
        protected TraceListener(
            string name,
            IEventFilter? filter,
            bool showEventType,
            bool showEventId)
        {
            this.name = name;
            this.filter = filter;
            this.showEventType = showEventType;
            this.showEventId = showEventId;
        }

        /// <summary>
        /// Gets a value indicating whether there is a custom event filter.this.
        /// </summary>
        public bool HasFilter()
        {
            return this.filter != null;
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show or hide the event type.
        /// </summary>
        public bool GetShowEventType()
        {
            return this.showEventType;
        }

        /// <summary>
        /// Gets or sets a value indicating whether to show or hide the event id.
        /// </summary>
        public bool GetShowEventId()
        {
            return this.showEventId;
        }

        public void SetShowEventId(bool value)
        {
            this.showEventId = value;
        }

        /// <summary>
        /// All other TraceEvent methods come through this one.
        /// </summary>
        /// <param name="eventType">The event type.</param>
        /// <param name="id">The event id.</param>
        /// <param name="message">The event message.</param>
        public void TraceEvent(
            TraceEventFlag eventType,
            int id,
            string message)
        {
            if (this.filter != null && !this.filter.ShouldTrace(eventType))
            {
                return;
            }

            // Build up the resulting message with required header/footer
            var builder = new StringBuilder();
            this.WriteHeader(builder, eventType, id);
            builder.Append(message);

            bool isEmpty = builder.Length == 0;
            if (isEmpty)
                this.WriteLine(string.Empty);
            else
                this.WriteLine(builder.ToString());
        }

        /// <summary>
        /// Implementation dependant write methods.
        /// </summary>
        /// <param name="message">The message.</param>
        protected abstract void WriteLine(string message);

        ///// <summary>
        ///// Trace Event.
        ///// </summary>
        //// void TraceEvent(
        ////	 string source,
        ////	 TraceEventFlag eventType,
        ////	 int id,
        ////	 string format,
        ////	 params object[] args)
        //// {
        ////	 if (Filter != null && !Filter.ShouldTrace( source, eventType, id, format, args, null, null))
        ////	 {
        ////		 return;
        ////	 }

        ////	 WriteHeader(source, eventType, id);
        ////	 if (args != null)
        ////	 {
        ////		 WriteLine(string.Format(CultureInfo.InvariantCulture, format, args));
        ////	 }
        ////	 else
        ////	 {
        ////		 WriteLine(format);
        ////	 }
        //// }

        /// <summary>
        /// Write the header to the target listener.
        /// </summary>
        private void WriteHeader(
            StringBuilder builder,
            TraceEventFlag eventType,
            int id)
        {
            if (this.GetShowEventType())
            {
                switch (eventType)
                {
                    case TraceEventFlag.HighPriority:
                        builder.Append("HIGH");
                        break;
                    case TraceEventFlag.Information:
                        builder.Append("INFO");
                        break;
                    case TraceEventFlag.Diagnostic:
                        builder.Append("DIAG");
                        break;
                    case TraceEventFlag.Warning:
                        builder.Append("WARN");
                        break;
                    case TraceEventFlag.Error:
                        builder.Append("ERRO");
                        break;
                    case TraceEventFlag.Critical:
                        builder.Append("CRIT");
                        break;
                    default:
                        builder.Append("UNKN");
                        break;
                }

                builder.Append(": ");
            }

            if (this.GetShowEventId())
            {
                builder.Append($"{id}>");
            }
        }
    }
}
