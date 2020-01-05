// <copyright file="BuildSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildState.h"

namespace Soup::Build
{
	/// <summary>
	/// The build system implementation
	/// </summary>
	export class BuildSystem : public IBuildSystem
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildSystem"/> class.
		/// </summary>
		BuildSystem() :
			_tasks()
		{
		}

		/// <summary>
		/// Register task
		/// </summary>
		OperationResult RegisterTask(IBuildTask* task) noexcept override final
		{
			try
			{
				Log::Diag(std::string("RegisterTask: ") + task->GetName());
				_tasks.push_back(task);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		void Execute(IBuildState& state)
		{
			for (auto& task : _tasks)
			{
				Log::Info(std::string("TaskStart: ") + task->GetName());
				auto status = task->Execute(state);
				if (status != 0)
				{
					Log::Error("TaskFailed: " + std::to_string(status));
				}
				else
				{
					Log::Info(std::string("TaskDone: ") + task->GetName());
				}
			}
		}

	private:
		std::vector<Memory::Reference<IBuildTask>> _tasks;
	};
}
