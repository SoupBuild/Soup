// <copyright file="OperationInfo.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	internal record OperationId(uint value);

	internal class CommandInfo : IEquatable<CommandInfo>
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		public CommandInfo() :
			this(new Path(), new Path(), string.Empty)
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		public CommandInfo(
			Path workingDirectory,
			Path executable,
			string arguments)
		{
			WorkingDirectory = workingDirectory;
			Executable = executable;
			Arguments = arguments;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		public bool Equals(CommandInfo? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;
			return WorkingDirectory == rhs.WorkingDirectory &&
				Executable == rhs.Executable &&
				Arguments == rhs.Arguments;
		}

		public override bool Equals(object? rhs)
		{
			return Equals(rhs as CommandInfo);
		}

		public override int GetHashCode()
		{
			return (WorkingDirectory.GetHashCode() * 0x100000) + (Executable.GetHashCode() * 0x1000) + Arguments.GetHashCode();
		}

		public static bool operator ==(CommandInfo lhs, CommandInfo rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
		}

		public static bool operator !=(CommandInfo lhs, CommandInfo rhs)
		{
			return !(lhs == rhs);
		}

		public Path WorkingDirectory { get; init; }
		public Path Executable { get; init; }
		public string Arguments { get; init; }
	}

	internal class OperationInfo : IEquatable<OperationInfo>
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		public OperationInfo() :
			this(
				new OperationId(0),
				string.Empty,
				new CommandInfo(),
				new List<FileId>(),
				new List<FileId>(),
				new List<OperationId>(),
				0,
				false,
				new List<FileId>(),
				new List<FileId>())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		public OperationInfo(
			OperationId id,
			string title,
			CommandInfo command,
			IList<FileId> declaredInput,
			IList<FileId> declaredOutput) :
			this(
				id,
				title,
				command,
				declaredInput,
				declaredOutput,
				new List<OperationId>(),
				0,
				false,
				new List<FileId>(),
				new List<FileId>())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		public OperationInfo(
			OperationId id,
			string title,
			CommandInfo command,
			IList<FileId> declaredInput,
			IList<FileId> declaredOutput,
			IList<OperationId> children,
			uint dependencyCount,
			bool wasSuccessfulRun,
			IList<FileId> observedInput,
			IList<FileId> observedOutput)
		{
			Id = id;
			Title = title;
			Command = command;
			DeclaredInput = declaredInput;
			DeclaredOutput = declaredOutput;
			Children = children;
			DependencyCount = dependencyCount;
			WasSuccessfulRun = wasSuccessfulRun;
			ObservedInput = observedInput;
			ObservedOutput = observedOutput;
		}

		public bool Equals(OperationInfo? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;
			return Id == rhs.Id &&
				Title == rhs.Title &&
				Command == rhs.Command &&
				DeclaredInput == rhs.DeclaredInput &&
				DeclaredOutput == rhs.DeclaredOutput &&
				Children == rhs.Children &&
				DependencyCount == rhs.DependencyCount &&
				WasSuccessfulRun == rhs.WasSuccessfulRun &&
				ObservedInput == rhs.ObservedInput &&
				ObservedOutput == rhs.ObservedOutput;
		}

		public override bool Equals(object? rhs)
		{
			return Equals(rhs as OperationInfo);
		}

		public override int GetHashCode()
		{
			return (Id.GetHashCode() * 0x100000) + (Title.GetHashCode() * 0x1000) + Command.GetHashCode();
		}

		public static bool operator ==(OperationInfo lhs, OperationInfo rhs)
		{
			if (ReferenceEquals(lhs, null))
				return ReferenceEquals(rhs, null);
			return lhs.Equals(rhs);
		}

		public static bool operator !=(OperationInfo lhs, OperationInfo rhs)
		{
			return !(lhs == rhs);
		}

		public OperationId Id { get; init; }
		public string Title { get; init; }
		public CommandInfo Command { get; init; }
		public IList<FileId> DeclaredInput { get; init; }
		public IList<FileId> DeclaredOutput { get; init; }
		public IList<OperationId> Children { get; init; }
		public uint DependencyCount { get; set; }
		public bool WasSuccessfulRun { get; init; }
		public IList<FileId> ObservedInput { get; init; }
		public IList<FileId> ObservedOutput { get; init; }
	}
}