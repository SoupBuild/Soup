// <copyright file="BuildHistory.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Execute
{
	export class OperationInfo
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo() :
			Command(),
			Input(),
			Output()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationInfo"/> class.
		/// </summary>
		OperationInfo(std::string command, std::vector<Path> input, std::vector<Path> output) :
			Command(std::move(command)),
			Input(std::move(input)),
			Output(std::move(output))
		{
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator ==(const OperationInfo& rhs) const
		{
			return Command == rhs.Command &&
				Input == rhs.Input &&
				Output == rhs.Output;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const OperationInfo& rhs) const
		{
			return Command != rhs.Command ||
				Input != rhs.Input ||
				Output != rhs.Output;
		}

		std::string Command;
		std::vector<Path> Input;
		std::vector<Path> Output;
	};

	export class BuildHistory
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildHistory"/> class.
		/// </summary>
		BuildHistory() :
			_operations()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="BuildHistory"/> class.
		/// </summary>
		BuildHistory(std::vector<OperationInfo> operations)
		{
			_operations.clear();
			for (auto& info : operations)
			{
				_operations.emplace(info.Command, std::move(info));
			}
		}

		/// <summary>
		/// Get Operations
		/// </summary>
		const std::unordered_map<std::string, OperationInfo>& GetOperations() const
		{
			return _operations;
		}

		/// <summary>
		/// Find an operation info
		/// </summary>
		bool TryFindOperationInfo(const std::string& command, const OperationInfo*& operation) const
		{
			auto findResult = _operations.find(command);
			if (findResult != _operations.end())
			{
				operation = &findResult->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Add an operation info
		/// </summary>
		void AddOperationInfo(OperationInfo operation)
		{
			auto result = _operations.emplace(operation.Command, std::move(operation));
			// TODO: Verify unique when operations are required to be unique
			/*if (!result.second)
				throw std::runtime_error("The provided command already exists in the build history.");*/
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const BuildHistory& rhs) const
		{
			return _operations != rhs._operations;
		}

	private:
		std::unordered_map<std::string, OperationInfo> _operations;
	};
}
