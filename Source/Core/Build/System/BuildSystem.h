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
			_state(),
			_tasks()
		{
		}

		/// <summary>
		/// Register task
		/// </summary>
		void RegisterTask(std::shared_ptr<IBuildTask> task) override final
		{
			Log::Diag(std::string("RegisterTask: ") + task->GetName());
			_tasks.push_back(std::move(task));
		}

		/// <summary>
		/// Get the set of added include paths
		/// </summary>
		void Execute()
		{
			for (auto& task : _tasks)
			{
				Log::Info(std::string("Task: ") + task->GetName());
				task->Execute(_state);
			}
		}

		BuildState& GetState()
		{
			return _state;
		}

	private:
		BuildState _state;
		std::vector<std::shared_ptr<IBuildTask>> _tasks;
	};
}
