// <copyright file="BuildSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildState.h"

namespace Soup::Build::Runtime
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
				Memory::Reference<IBuildTask> ourTask = task;

				std::string taskName = ourTask->GetName();
				Log::Diag("RegisterTask: " + taskName);

				std::stringstream runBeforeMessage;
				runBeforeMessage << "RunBefore [";
				bool isFirst = true;
				for (auto& name : Extensions::StringListWrapper(ourTask->GetRunBeforeList()).CopyAsStringVector())
				{
					if (!isFirst)
						runBeforeMessage << ", ";

					runBeforeMessage << "\"" << name << "\"";
					isFirst = false;
				}

				runBeforeMessage << "]";
				Log::Diag(runBeforeMessage.str());

				std::stringstream runAfterMessage;
				runAfterMessage << "RunAfter [";
				isFirst = true;
				for (auto& name : Extensions::StringListWrapper(ourTask->GetRunAfterList()).CopyAsStringVector())
				{
					if (!isFirst)
						runAfterMessage << ", ";

					runAfterMessage << "\"" << name << "\"";
					isFirst = false;
				}

				runAfterMessage << "]";
				Log::Diag(runAfterMessage.str());

				auto insertResult = _tasks.try_emplace(taskName, ourTask);
				if (!insertResult.second)
				{
					Log::HighPriority("A task with the provided name has already been registered: " + taskName);
					return -2;
				}

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
		void Execute(BuildState& state)
		{
			for (auto& task : _tasks)
			{
				Log::Info("TaskStart: " + task.first);
				auto status = task.second->Execute(state);
				if (status != 0)
				{
					Log::Error("TaskFailed: " + std::to_string(status));
				}
				else
				{
					Log::Info("TaskDone: " + task.first);
				}

				state.LogActive();
			}
		}

	private:
		std::unordered_map<std::string, Memory::Reference<IBuildTask>> _tasks;
	};
}
