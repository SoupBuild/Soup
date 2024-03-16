// <copyright file="CommandResult.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		WasSuccessfulRun = wasSuccessfulRun;
		EvaluateTime = evaluateTime;
		ObservedInput = observedInput;
		ObservedOutput = observedOutput;
	}

	public bool Equals(OperationResult? other)
	{
		if (other is null)
			return false;

		var result = WasSuccessfulRun == other.WasSuccessfulRun &&
			EvaluateTime == other.EvaluateTime &&
			Enumerable.SequenceEqual(ObservedInput, other.ObservedInput) &&
			Enumerable.SequenceEqual(ObservedOutput, other.ObservedOutput);

		return result;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as OperationResult);
	}

	public override int GetHashCode()
	{
		return (WasSuccessfulRun.GetHashCode() * 0x100000) + (EvaluateTime.GetHashCode() * 0x1000);
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