// <copyright file="CommandResult.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities;

public class OperationResult : IEquatable<OperationResult>
{
	/// <summary>
	/// Initializes a new instance of the <see cref="OperationResult"/> class.
	/// </summary>
	public OperationResult(
		bool wasSuccessfulRun,
		DateTime evaluateTime,
		IList<FileId> observedInput,
		IList<FileId> observedOutput)
	{
		this.WasSuccessfulRun = wasSuccessfulRun;
		this.EvaluateTime = evaluateTime;
		this.ObservedInput = observedInput;
		this.ObservedOutput = observedOutput;
	}

	public bool Equals(OperationResult? other)
	{
		if (other is null)
			return false;

		var result = this.WasSuccessfulRun == other.WasSuccessfulRun &&
			this.EvaluateTime == other.EvaluateTime &&
			Enumerable.SequenceEqual(this.ObservedInput, other.ObservedInput) &&
			Enumerable.SequenceEqual(this.ObservedOutput, other.ObservedOutput);

		return result;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as OperationResult);
	}

	public override int GetHashCode()
	{
		return (this.WasSuccessfulRun.GetHashCode() * 0x100000) + (this.EvaluateTime.GetHashCode() * 0x1000);
	}

	public static bool operator ==(OperationResult? lhs, OperationResult? rhs)
	{
		if (lhs is null)
			return rhs is null;
		return lhs.Equals(rhs);
	}

	public static bool operator !=(OperationResult? lhs, OperationResult? rhs)
	{
		return !(lhs == rhs);
	}

	public bool WasSuccessfulRun { get; init; }
	public DateTime EvaluateTime { get; init; }
	public IList<FileId> ObservedInput { get; init; }
	public IList<FileId> ObservedOutput { get; init; }
}