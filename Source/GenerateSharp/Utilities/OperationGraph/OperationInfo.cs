// <copyright file="OperationInfo.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using Path = Opal.Path;

namespace Soup.Build.Utilities;

public record OperationId(uint Value);

public class CommandInfo : IEquatable<CommandInfo>
{
	/// <summary>
	/// Initializes a new instance of the <see cref="CommandInfo"/> class.
	/// </summary>
	public CommandInfo() :
		this(new Path(), new Path(), [])
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
	public bool Equals(CommandInfo? other)
	{
		if (other is null)
			return false;
		return WorkingDirectory == other.WorkingDirectory &&
			Executable == other.Executable &&
			Arguments == other.Arguments;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as CommandInfo);
	}

	public override int GetHashCode()
	{
		return (WorkingDirectory.GetHashCode() * 0x100000) + (Executable.GetHashCode() * 0x1000) + Arguments.GetHashCode();
	}

	public static bool operator ==(CommandInfo lhs, CommandInfo other)
	{
		if (lhs is null)
			return other is null;
		return lhs.Equals(other);
	}

	public static bool operator !=(CommandInfo lhs, CommandInfo other)
	{
		return !(lhs == other);
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
			[],
			[],
			[],
			[],
			[],
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
			[],
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

	public bool Equals(OperationInfo? other)
	{
		if (other is null)
			return false;

		var result = Id == other.Id &&
			Title == other.Title &&
			Command == other.Command &&
			Enumerable.SequenceEqual(DeclaredInput, other.DeclaredInput) &&
			Enumerable.SequenceEqual(DeclaredOutput, other.DeclaredOutput) &&
			Enumerable.SequenceEqual(ReadAccess, other.ReadAccess) &&
			Enumerable.SequenceEqual(WriteAccess, other.WriteAccess) &&
			Enumerable.SequenceEqual(Children, other.Children) &&
			DependencyCount == other.DependencyCount;

		return result;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as OperationInfo);
	}

	public override int GetHashCode()
	{
		return (Id.GetHashCode() * 0x100000) + (Title.GetHashCode(StringComparison.InvariantCulture) * 0x1000) + Command.GetHashCode();
	}

	public static bool operator ==(OperationInfo lhs, OperationInfo other)
	{
		if (lhs is null)
			return other is null;
		return lhs.Equals(other);
	}

	public static bool operator !=(OperationInfo lhs, OperationInfo other)
	{
		return !(lhs == other);
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