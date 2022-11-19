// <copyright file="IBuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	public enum TraceLevel : uint
	{
		/// <summary>
		/// Exceptional state that will fail the build
		/// </summary>
		Error = 1,

		/// <summary>
		/// A possible issue in the build that may be fine to continue
		/// </summary>
		Warning = 2,

		/// <summary>
		/// Highest level of logging that will be on in all but the quiet logs
		/// </summary>
		HighPriority = 3,

		/// <summary>
		/// Important information that will be on in verbose logs. May help users investigate what occurred during a build.
		/// </summary>
		Information = 4,

		/// <summary>
		/// The most detailed of logs that will only be useful for detailed investigations into runtime issues for build engineers. Diagnostic log level.
		/// </summary>
		Debug = 5,
	}

	public interface IBuildState
	{
		/// <summary>
		/// Gets a reference to the active state.
		/// </summary>
		IValueTable ActiveState { get; }

		/// <summary>
		/// Gets a reference to the shared state. All of these properties will be.
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable SharedState { get; }

		/// <summary>
		/// Create a build operation.
		/// </summary>
		/// <param name="title">The title.</param>
		/// <param name="executable">The executable.</param>
		/// <param name="arguments">The arguments.</param>
		/// <param name="workingDirectory">The workingDirectory.</param>
		/// <param name="declaredInput">The declaredInput.</param>
		/// <param name="declaredOutput">The declaredOutput.</param>
		void CreateOperation(
			string title,
			string executable,
			string arguments,
			string workingDirectory,
			string[] declaredInput,
			string[] declaredOutput);

		/// <summary>
		/// Log a message to the build system.
		/// </summary>
		/// <param name="level">The trace level.</param>
		/// <param name="message">The message.</param>
		void LogTrace(TraceLevel level, string message);
	}
}
