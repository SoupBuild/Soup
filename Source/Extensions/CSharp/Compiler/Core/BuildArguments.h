// <copyright file="BuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::Compiler
{
	/// <summary>
	/// The enumeration of target types
	/// </summary>
	export enum class BuildTargetType
	{
		/// <summary>
		/// Executable
		/// </summary>
		Executable,

		/// <summary>
		/// Library
		/// </summary>
		Library,
	};

	std::string ToString(BuildTargetType value)
	{
		switch (value)
		{
			case BuildTargetType::Executable:
				return "Executable";
			case BuildTargetType::Library:
				return "Library";
			default:
				throw std::runtime_error("Unknown BuildTargetType");
		}
	}

	/// <summary>
	/// The set of build arguments
	/// </summary>
	export struct BuildArguments
	{
		/// <summary>
		/// Gets or sets the target name
		/// </summary>
		std::string TargetName;

		/// <summary>
		/// Gets or sets the target architecture
		/// </summary>
		std::string TargetArchitecture;

		/// <summary>
		/// Gets or sets the target type
		/// </summary>
		BuildTargetType TargetType;

		/// <summary>
		/// Gets or sets the working directory
		/// </summary>
		Path WorkingDirectory;

		/// <summary>
		/// Gets or sets the output object directory
		/// </summary>
		Path ObjectDirectory;

		/// <summary>
		/// Gets or sets the output binary directory
		/// </summary>
		Path BinaryDirectory;

		/// <summary>
		/// Gets or sets the list of source files
		/// Note: These files can be plain old translation units 
		/// or they can be module implementation units
		/// </summary>
		std::vector<Path> SourceFiles;

		/// <summary>
		/// Gets or sets the list of library paths
		/// </summary>
		std::vector<Path> LibraryPaths;

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		std::vector<std::string> PreprocessorDefinitions;

		/// <summary>
		/// Gets or sets the list of runtime dependencies
		/// </summary>
		std::vector<Path> RuntimeDependencies;

		/// <summary>
		/// Gets or sets a value that indicates if optimization is enabled
		/// </summary>
		bool EnableOptimization;

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		bool GenerateSourceDebugInfo;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const BuildArguments& rhs) const
		{
			return TargetName == rhs.TargetName &&
				TargetArchitecture == rhs.TargetArchitecture &&
				TargetType == rhs.TargetType &&
				WorkingDirectory == rhs.WorkingDirectory &&
				ObjectDirectory == rhs.ObjectDirectory &&
				BinaryDirectory == rhs.BinaryDirectory &&
				SourceFiles == rhs.SourceFiles &&
				LibraryPaths == rhs.LibraryPaths &&
				PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
				EnableOptimization == rhs.EnableOptimization &&
				GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo;
		}

		bool operator !=(const BuildArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
