// <copyright file="OperationInfo.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public record OperationId(uint value);

	public class CommandInfo : IEquatable<CommandInfo>
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		public CommandInfo() :
			this(new Path(), new Path(), new List<string>())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="CommandInfo"/> class.
		/// </summary>
		public CommandInfo(
			Path workingDirectory,
			Path executable,
			IReadOnlyList<string> arguments)
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
		public IReadOnlyList<string> Arguments { get; init; }
	}

	public class OperationInfo : IEquatable<OperationInfo>
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
				new List<FileId>(),
				new List<FileId>(),
				new List<OperationId>(),
				0)
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
			IList<FileId> readAccess,
			IList<FileId> writeAccess) :
			this(
				id,
				title,
				command,
				declaredInput,
				declaredOutput,
				readAccess,
				writeAccess,
				new List<OperationId>(),
				0)
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
			IList<FileId> readAccess,
			IList<FileId> writeAccess,
			IList<OperationId> children,
			uint dependencyCount)
		{
			Id = id;
			Title = title;
			Command = command;
			DeclaredInput = declaredInput;
			DeclaredOutput = declaredOutput;
			ReadAccess = readAccess;
			WriteAccess = writeAccess;
			Children = children;
			DependencyCount = dependencyCount;
		}

		public bool Equals(OperationInfo? rhs)
		{
			if (ReferenceEquals(rhs, null))
				return false;

			var result = Id == rhs.Id &&
				Title == rhs.Title &&
				Command == rhs.Command &&
				Enumerable.SequenceEqual(DeclaredInput, rhs.DeclaredInput) &&
				Enumerable.SequenceEqual(DeclaredOutput, rhs.DeclaredOutput) &&
				Enumerable.SequenceEqual(ReadAccess, rhs.ReadAccess) &&
				Enumerable.SequenceEqual(WriteAccess, rhs.WriteAccess) &&
				Enumerable.SequenceEqual(Children, rhs.Children) &&
				DependencyCount == rhs.DependencyCount;

			return result;
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
		public IList<FileId> ReadAccess { get; init; }
		public IList<FileId> WriteAccess { get; init; }
		public IList<OperationId> Children { get; init; }
		public uint DependencyCount { get; set; }
	}
}