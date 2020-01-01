// <copyright file="IBuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildSystemResult.h"
#include "IBuildPropertyValue.h"
#include "IBuildPropertyList.h"

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
		virtual void AddBuildNode(std::shared_ptr<BuildGraphNode> node) noexcept = 0;
		
		/// <summary>
		/// Property access methods
		/// </summary>
		virtual BuildSystemResult TryHasPropertyValue(const char* name, bool& result) const noexcept = 0;
		virtual BuildSystemResult TryGetPropertyValue(const char* name, IBuildPropertyValue*& result) noexcept = 0;
		virtual BuildSystemResult TryCreatePropertyValue(const char* name, IBuildPropertyValue*& result) noexcept = 0;

		virtual BuildSystemResult TryHasPropertyStringList(const char* name, bool& result) const noexcept = 0;
		virtual BuildSystemResult TryGetPropertyStringList(const char* name, IBuildPropertyList<const char*>*& result) noexcept = 0;
		virtual BuildSystemResult TryCreatePropertyStringList(const char* name, IBuildPropertyList<const char*>*& result) noexcept = 0;
	};
}
