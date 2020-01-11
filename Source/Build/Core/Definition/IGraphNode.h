// <copyright file="IGraphNode.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IList.h"

namespace Soup::Build
{
	/// <summary>
	/// A graph node that represents a single operation in the build
	/// </summary>
	export class IGraphNode : public Memory::IReferenceCounted
	{
	public:
		virtual int64_t GetId() const noexcept = 0;

		virtual const char* GetTitle() const noexcept = 0;
		virtual OperationResult TrySetTitle(const char* value) noexcept = 0;

		virtual const char* GetProgram() const noexcept = 0;
		virtual OperationResult TrySetProgram(const char* value) noexcept = 0;

		virtual const char* GetArguments() const noexcept = 0;
		virtual OperationResult TrySetArguments(const char* value) noexcept = 0;

		virtual const char* GetWorkingDirectory() const noexcept = 0;
		virtual OperationResult TrySetWorkingDirectory(const char* value) noexcept = 0;

		virtual IList<const char*>& GetInputFileList() noexcept = 0;

		virtual IList<const char*>& GetOutputFileList() noexcept = 0;

		virtual IList<IGraphNode*>& GetChildList() noexcept = 0;
	};
}
