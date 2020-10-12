// <copyright file="BuildStateWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildOperation.h"
#include "ValueTableWrapper.h"

namespace Soup::Build::Utilities
{
	/// <summary>
	/// Build Property Bag Extension used to make the IBuildState interface more usable
	/// </summary>
	export class BuildStateWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildStateWrapper class
		/// </summary>
		BuildStateWrapper(IBuildState& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		ValueTableWrapper GetActiveState() noexcept
		{
			return ValueTableWrapper(_value.GetActiveState());
		}

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		ValueTableWrapper GetSharedState() noexcept
		{
			return ValueTableWrapper(_value.GetSharedState());
		}

		/// <summary>
		/// Create a build operation
		/// </summary>
		void CreateOperation(const BuildOperation& operation)
		{
			auto declaredInput = ReadOnlyStringList(operation.DeclaredInput);
			auto declaredOutput = ReadOnlyStringList(operation.DeclaredOutput);
			auto status = _value.TryCreateOperation(
				operation.Title.c_str(),
				operation.Executable.ToString().c_str(),
				operation.Arguments.c_str(),
				operation.WorkingDirectory.ToString().c_str(),
				declaredInput,
				declaredOutput);
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryCreateOperation Failed");
		}

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		void LogDebug(std::string_view message)
		{
			auto status = _value.TryLogTrace(TraceLevel::Debug, message.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryLogTrace Failed");
		}

		void LogInfo(std::string_view message)
		{
			auto status = _value.TryLogTrace(TraceLevel::Information, message.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryLogTrace Failed");
		}

		void LogHighPriority(std::string_view message)
		{
			auto status = _value.TryLogTrace(TraceLevel::HighPriority, message.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryLogTrace Failed");
		}

		void LogWarning(std::string_view message)
		{
			auto status = _value.TryLogTrace(TraceLevel::Warning, message.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryLogTrace Failed");
		}

		void LogError(std::string_view message)
		{
			auto status = _value.TryLogTrace(TraceLevel::Error, message.data());
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TryLogTrace Failed");
		}

	private:
		IBuildState& _value;
	};
}
