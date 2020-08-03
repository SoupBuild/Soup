// <copyright file="IBuildOperation.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IList.h"
#include "IReadOnlyList.h"

namespace Soup::Build
{
	/// <summary>
	/// A build operation that represents a single operation invocation in the build execution
	/// </summary>
	export class IBuildOperation : public Memory::IReferenceCounted
	{
	public:
		virtual const char* GetTitle() const noexcept = 0;
		virtual const char* GetExecutable() const noexcept = 0;
		virtual const char* GetArguments() const noexcept = 0;
		virtual const char* GetWorkingDirectory() const noexcept = 0;

		virtual const IReadOnlyList<const char*>& GetInputFileList() const noexcept = 0;
		virtual const IReadOnlyList<const char*>& GetOutputFileList() const noexcept = 0;

		// Allow read write access to the child list to allow adding dependent operations
		virtual IList<IBuildOperation*>& GetChildList() noexcept = 0;
	};
}
