// <copyright file="IBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"
#include "IBuildState.h"

namespace Soup::Build
{
	/// <summary>
	/// Build Task Extension interface that will be implemented by the build extensions
	/// and will be executed by the build system.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export class IBuildTask : public virtual Opal::Memory::IReferenceCounted
	{
	public:
		virtual const char* GetName() const noexcept = 0;
		virtual IList<const char*>& GetRunBeforeList() noexcept = 0;
		virtual IList<const char*>& GetRunAfterList() noexcept = 0;

		virtual OperationResult Execute(IBuildState& state) noexcept = 0;
	};
}
