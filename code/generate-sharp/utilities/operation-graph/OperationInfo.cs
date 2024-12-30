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
		this.WorkingDirectory = workingDirectory;
		this.Executable = executable;
		this.Arguments = arguments;
	}

	/// <summary>
	/// Equality operator
	/// </summary>
	public bool Equals(CommandInfo? other)
	{
		if (other is null)
			return false;
		return this.WorkingDirectory == other.WorkingDirectory &&
			this.Executable == other.Executable &&
			this.Arguments == other.Arguments;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as CommandInfo);
	}

	public override int GetHashCode()
	{
		return (this.WorkingDirectory.GetHashCode() * 0x100000) + (this.Executable.GetHashCode() * 0x1000) + this.Arguments.GetHashCode();
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
		this.Id = id;
		this.Title = title;
		this.Command = command;
		this.DeclaredInput = declaredInput;
		this.DeclaredOutput = declaredOutput;
		this.ReadAccess = readAccess;
		this.WriteAccess = writeAccess;
		this.Children = children;
		this.DependencyCount = dependencyCount;
	}

	public bool Equals(OperationInfo? other)
	{
		if (other is null)
			return false;

		var result = this.Id == other.Id &&
			this.Title == other.Title &&
			this.Command == other.Command &&
			Enumerable.SequenceEqual(this.DeclaredInput, other.DeclaredInput) &&
			Enumerable.SequenceEqual(this.DeclaredOutput, other.DeclaredOutput) &&
			Enumerable.SequenceEqual(this.ReadAccess, other.ReadAccess) &&
			Enumerable.SequenceEqual(this.WriteAccess, other.WriteAccess) &&
			Enumerable.SequenceEqual(this.Children, other.Children) &&
			this.DependencyCount == other.DependencyCount;

		return result;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as OperationInfo);
	}

	public override int GetHashCode()
	{
		return (this.Id.GetHashCode() * 0x100000) + (this.Title.GetHashCode(StringComparison.InvariantCulture) * 0x1000) + this.Command.GetHashCode();
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