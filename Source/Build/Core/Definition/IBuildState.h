// <copyright file="IBuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"
#include "IPropertyBag.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface that is the core object granting
	/// build extensions access to the shared build state.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	class IBuildState
	{
	public:
		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		virtual OperationResult TryRegisterRootNode(IGraphNode* node) noexcept = 0;
		virtual OperationResult TryCreateNode(IGraphNode*& node) noexcept = 0;

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		virtual IPropertyBag& GetActiveState() noexcept = 0;

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		virtual IPropertyBag& GetParentState() noexcept = 0;
	};
}
