// <copyright file="MockBuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::TestExtensions
{
	/// <summary>
	/// A mock Build State interface implementation to track incoming calls and setup input state
	/// </summary>
	export class MockBuildState : public IBuildState
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState() :
			_activeState(),
			_sharedState(),
			_operations()
		{
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		IValueTable& GetActiveState() noexcept override final
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable& GetSharedState() noexcept override final
		{
			return _sharedState;
		}

		/// <summary>
		/// Create a build operation
		/// </summary>
		ApiCallResult TryCreateOperation(
			const char* title,
			const char* executable,
			const char* arguments,
			const char* workingDirectory,
			IReadOnlyList<const char*>& declaredInput,
			IReadOnlyList<const char*>& declaredOutput) noexcept override final
		{
			try
			{
				_graphGenerator.CreateOperation(
					title,
					Path(executable),
					arguments,
					Path(workingDirectory),
					Utilities::ReadOnlyStringListWrapper(declaredInput).CopyAsPathVector(),
					Utilities::ReadOnlyStringListWrapper(declaredOutput).CopyAsPathVector());

				// Build up the operation unique command
				auto commandInfo = CommandInfo(
					Path(workingDirectory),
					Path(executable),
					arguments);

				// Generate a unique id for this new operation
				auto operationId = _operations.size() + 1;

			// Build up the declared build operation
			auto declaredInputFileIds = _fileSystemState.ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			auto declaredOutputFileIds = _fileSystemState.ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			_graph.AddOperation(
				OperationInfo(
					operationId,
					std::move(title),
					std::move(commandInfo),
					std::move(declaredInputFileIds),
					std::move(declaredOutputFileIds)));

				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		ApiCallResult TryLogTrace(TraceLevel level, const char* message) noexcept override final
		{
			try
			{
				switch (level)
				{
				case TraceLevel::Error:
					Log::Error(message);
					break;
				case TraceLevel::Warning:
					Log::Warning(message);
					break;
				case TraceLevel::HighPriority:
					Log::HighPriority(message);
					break;
				case TraceLevel::Information:
					Log::Info(message);
					break;
				case TraceLevel::Debug:
					Log::Diag(message);
					break;
				default:
					return ApiCallResult::Error;
				}

				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

	private:
		ValueTable _activeState;
		ValueTable _sharedState;
		std::vector<OperationInfo> _operations;
	};
}
