// <copyright file="BuildStateWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Definition/IBuildState.h"
#include "GraphNodeWrapper.h"
#include "PropertyBagWrapper.h"

namespace Soup::Build
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
		/// Build Graph Access Methods
		/// </summary>
		void RegisterRootNode(GraphNodeWrapper& node) noexcept
		{
			auto status = _value.TryRegisterRootNode(node.GetRaw());
			if (status != 0)
				throw std::runtime_error("TryRegisterRootNode Failed");
		}

		GraphNodeWrapper CreateNode()
		{
			IGraphNode* result = nullptr;
			auto status = _value.TryCreateNode(result);

			// Store the out result to ensure we do not leak it by accident
			Memory::Reference<IGraphNode> resultReference = result;

			if (status != 0)
				throw std::runtime_error("TryCreateNode Failed");

			return GraphNodeWrapper(resultReference);
		}

		GraphNodeWrapper CreateNode(
			const std::string& title,
			const Path& program,
			const std::string& arguments,
			const Path& workingDirectory,
			const std::vector<Path>& inputFiles,
			const std::vector<Path>& outputFiles)
		{
			auto node = CreateNode();

			node.SetTitle(title);
			node.SetProgram(program.ToString());
			node.SetArguments(arguments);
			node.SetWorkingDirectory(workingDirectory.ToString());
			node.GetInputFileList().SetAll(inputFiles);
			node.GetOutputFileList().SetAll(outputFiles);

			return node;
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		PropertyBagWrapper GetActiveState() noexcept
		{
			return PropertyBagWrapper(_value.GetActiveState());
		}

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		PropertyBagWrapper GetParentState() noexcept
		{
			return PropertyBagWrapper(_value.GetParentState());
		}

	private:
		IBuildState& _value;
	};
}
