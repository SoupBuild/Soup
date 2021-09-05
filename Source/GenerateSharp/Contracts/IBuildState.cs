// <copyright file="IBuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	public enum TraceLevel : uint
	{
		// Exceptional state that will fail the build
		Error = 1,

		// A possible issue in the build that may be fine to continue
		Warning = 2,

		// Highest level of logging that will be on in all but the quiet logs
		HighPriority = 3,

		// Important information that will be on in verbose logs. May help users investigate what occurred during a build.
		Information = 4,

		// The most detailed of logs that will only be useful for detailed investigations into runtime issues for build engineers. Diagnostic log level.
		Debug = 5,
	};

	public interface IBuildState
	{
		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		IValueTable ActiveState { get; }

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable SharedState { get; }

		/// <summary>
		/// Create a build operation
		/// </summary>
		void CreateOperation(
			string title,
			string executable,
			string arguments,
			string workingDirectory,
			string[] declaredInput,
			string[] declaredOutput);

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		void LogTrace(TraceLevel level, string message);
	}
}
