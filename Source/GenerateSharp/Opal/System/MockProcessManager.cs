// <copyright file="MockProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
    using global::System.Collections.Generic;

    /// <summary>
    /// The mock process manager
    /// TODO: Move into test project.
    /// </summary>
    public class MockProcessManager : IProcessManager
    {
        private int uniqueId;
        private List<string> requests;
        private Path processFileName;
        private Dictionary<string, string> executeResults;

        /// <summary>
        /// Initializes a new instance of the <see cref='MockProcessManager'/> class.
        /// </summary>
        public MockProcessManager()
        {
            this.uniqueId = 1;
            this.requests = new List<string>();
            this.processFileName = new Path("C:/testlocation/SoupCMDTest.exe");
            this.executeResults = new Dictionary<string, string>();
        }

        /// <summary>
        /// Initializes a new instance of the <see cref='MockProcessManager'/> class.
        /// </summary>
        /// <param name="processFileName">The process file name.</param>
        public MockProcessManager(Path processFileName)
        {
            this.uniqueId = 1;
            this.requests = new List<string>();
            this.processFileName = processFileName;
            this.executeResults = new Dictionary<string, string>();
        }

        /// <summary>
        /// Create a result.
        /// </summary>
        /// <param name="command">The command.</param>
        /// <param name="output">The output.</param>
        public void RegisterExecuteResult(string command, string output)
        {
            this.executeResults.Add(command, output);
        }

        /// <summary>
        /// Get the load requests.
        /// </summary>
        public IReadOnlyList<string> GetRequests()
        {
            return this.requests;
        }

        /// <summary>
        /// Gets the process file name.
        /// </summary>
        public Path GetCurrentProcessFileName()
        {
            this.requests.Add("GetCurrentProcessFileName");
            return this.processFileName;
        }

        /// <summary>
        /// Creates a process for the provided executable path.
        /// </summary>
        /// <param name="executable">The executable.</param>
        /// <param name="arguments">The arguments.</param>
        /// <param name="workingDirectory">The working directory.</param>
        public IProcess CreateProcess(
            Path executable,
            string arguments,
            Path workingDirectory)
        {
            var id = this.uniqueId++;
            var message = $"CreateProcess: {id} [{workingDirectory}] {executable} {arguments}";
            this.requests.Add(message);

            // Check if there is a registered output
            if (this.executeResults.TryGetValue(message, out var output))
            {
                return new MockProcess(
                    id,
                    this.requests,
                    0,
                    output,
                    string.Empty);
            }
            else
            {
                return new MockProcess(
                    id,
                    this.requests,
                    0,
                    string.Empty,
                    string.Empty);
            }
        }
    }
}
