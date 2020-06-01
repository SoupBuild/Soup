// <copyright file="BuildResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler
{
	/// <summary>
	/// The build result
	/// </summary>
	export struct BuildResult
	{
		/// <summary>
		/// Gets or sets the resulting root build operations
		/// </summary>
		std::vector<Soup::Build::Extensions::BuildOperationWrapper> BuildOperations;

		/// <summary>
		/// Gets or sets the list of module dependencies
		/// </summary>
		std::vector<Path> ModuleDependencies;

		/// <summary>
		/// Gets or sets the list of link libraries
		/// </summary>
		std::vector<Path> LinkDependencies;

		/// <summary>
		/// Gets or sets the list of runtime dependencies
		/// </summary>
		std::vector<Path> RuntimeDependencies;
	};
}
