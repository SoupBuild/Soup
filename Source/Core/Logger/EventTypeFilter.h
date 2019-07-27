// <copyright file="TraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "TraceListener.h"

namespace Soup
{
    export class EventTypeFilter : public IEventFilter
    {
    public:
        EventTypeFilter(TraceEventFlag eventTypes) :
            _types(static_cast<uint32_t>(eventTypes))
        {
        }

        void Disable(TraceEventFlag eventType)
        {
            _types &= ~static_cast<uint32_t>(eventType);
        }

        void Enable(TraceEventFlag eventType)
        {
            _types |= static_cast<uint32_t>(eventType);
        }

        bool IsEnabled(TraceEventFlag eventType)
        {
            auto typeValue = static_cast<uint32_t>(eventType);
            return ( _types & typeValue) == typeValue;
        }

        virtual bool ShouldTrace(TraceEventFlag eventType) override final
        {
            return IsEnabled(eventType);
        }

    private:
        uint32_t _types;
    };
}
