// <copyright file="IProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
    /// <summary>
    /// The process manager interface
    /// Interface mainly used to allow for unit testing client code.
    /// </summary>
    public interface IProcessManager
    {
        /// <summary>
        /// Gets the process file name.
        /// </summary>
        public abstract Path GetCurrentProcessFileName();

        /// <summary>
        /// Creates a process for the provided executable path.
        /// </summary>
        /// <param name="executable">The executable.</param>
        /// <param name="arguments">The arguments.</param>
        /// <param name="workingDirectory">The working directory.</param>
        public abstract IProcess CreateProcess(
            Path executable,
            string arguments,
            Path workingDirectory);
    }
}
