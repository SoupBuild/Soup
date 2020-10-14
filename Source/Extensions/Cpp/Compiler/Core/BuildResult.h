// <copyright file="BuildResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler
{
	/// <summary>
	/// The build result
	/// </summary>
	export struct BuildResult
	{
		/// <summary>
		/// Gets or sets the resulting root build operations
		/// </summary>
		std::vector<Build::Utilities::BuildOperation> BuildOperations;

		/// <summary>
		/// Gets or sets the list of module dependencies
		/// </summary>
		std::vector<Path> ModuleDependencies;

		/// <summary>
		/// Gets or sets the list of link libraries that downstream builds should use when linking
		/// </summary>
		std::vector<Path> LinkDependencies;

		/// <summary>
		/// Gets or sets the list of internal link libraries that were used to link the final result
		/// </summary>
		std::vector<Path> InternalLinkDependencies;

		/// <summary>
		/// Gets or sets the list of runtime dependencies
		/// </summary>
		std::vector<Path> RuntimeDependencies;
	};
}
