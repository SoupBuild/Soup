// <copyright file="PlatformProcessManagerImpl.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    typedef void OutputCallBackFunction(void* context, const char*, int);

    /// <summary>
    /// A platform specific process executable using system
    /// </summary>
    class PlatformProcessManagerImpl
    {
    public:
        /// <summary>
        /// Creates a process for the provided executable path
        /// </summary>
        static int Execute(
            const char* path,
            char* arguments,
            const char* workingDirectory,
            OutputCallBackFunction stdOutputCallback,
            void* stdOutputContext,
            OutputCallBackFunction stdErrorCallback,
            void* stdErrorContext);
    };
}
