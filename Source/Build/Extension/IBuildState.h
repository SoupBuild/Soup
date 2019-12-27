// <copyright file="IBuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class IBuildState
	{
	public:
		virtual bool HasProperty(const char* name) = 0;
		virtual const std::any& GetProperty(const char* name) = 0;
		virtual void SetProperty(const char* name, std::any value) = 0;

		virtual void AddBuildNode(std::shared_ptr<BuildGraphNode> node) = 0;
	};
}
