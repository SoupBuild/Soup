// <copyright file="TestTraceListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "TraceListener.h"

namespace Soup
{
    /// <summary>
    /// Test logger that wraps the base <see cref="TraceListener"/>
    /// </summary>
    export class TestTraceListener : public TraceListener
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref='TestTraceListener'/> class.
        /// </summary>
        TestTraceListener() :
            TraceListener("", nullptr, false, false),
            m_messages()
        {
        }

        /// <summary>
        /// Get the messages
        /// </summary>
        const std::vector<std::string>& GetMessages() const
        {
            return m_messages;
        }

        /// <summary>
        /// Writes a message
        /// </summary>
        virtual void Write(const std::string& message) override final
        {
            m_messages.push_back(message);
        }

        /// <summary>
        /// Writes a message and newline terminator
        /// </summary>
        virtual void WriteLine(const std::string& message) override final
        {
            auto line = message + "\n";
            m_messages.push_back(line);
        }

    private:
        std::vector<std::string> m_messages;
    };
}
