// <copyright file="OperationInfo.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "FileSystemState.h"
#include "CommandInfo.h"

using namespace std::chrono_literals;

namespace Soup::Core
{
	/// <summary>
	/// A node result that tacks the observed output from previous runs
	/// </summary>
	export class OperationResult
	{
	public:
		bool WasSuccessfulRun;
		std::chrono::time_point<std::chrono::file_clock> EvaluateTime;
		std::vector<FileId> ObservedInput;
		std::vector<FileId> ObservedOutput;

	public:
		OperationResult() :
			WasSuccessfulRun(false),
			EvaluateTime(std::chrono::time_point<std::chrono::file_clock>::min()),
			ObservedInput(),
			ObservedOutput()
		{
		}

		OperationResult(
			bool wasSuccessfulRun,
			std::chrono::time_point<std::chrono::file_clock> evaluateTime,
			std::vector<FileId> observedInput,
			std::vector<FileId> observedOutput) :
			WasSuccessfulRun(wasSuccessfulRun),
			EvaluateTime(evaluateTime),
			ObservedInput(std::move(observedInput)),
			ObservedOutput(std::move(observedOutput))
		{
		}

		bool operator ==(const OperationResult& rhs) const
		{
			return WasSuccessfulRun == rhs.WasSuccessfulRun &&
				EvaluateTime == rhs.EvaluateTime &&
				ObservedInput == rhs.ObservedInput &&
				ObservedOutput == rhs.ObservedOutput;
		}
	};
}