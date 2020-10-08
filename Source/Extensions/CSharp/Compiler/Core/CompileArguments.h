// <copyright file="CompilerArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::Compiler
{
	/// <summary>
	/// The enumeration of targets
	/// </summary>
	export enum class TargetType
	{
		/// <summary>
		/// Library
		/// </summary>
		Library,

		/// <summary>
		/// Executable
		/// </summary>
		Executable,
	};

	std::string ToString(TargetType value)
	{
		switch (value)
		{
			case TargetType::Library:
				return "Library";;
			case TargetType::Executable:
				return "Executable";
			default:
				throw std::runtime_error("Unknown LinkTarget");
		}
	}

	/// <summary>
	/// The set of standard compiler arguments
	/// </summary>
	export struct CompileArguments
	{
		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		Path TargetFile;

		/// <summary>
		/// Gets or sets the target type
		/// </summary>
		TargetType TargetType;

		/// <summary>
		/// Gets or sets a value indicating if optimization is enabled
		/// </summary>
		bool EnableOptimize;

		/// <summary>
		/// Gets or sets the root directory
		/// </summary>
		Path RootDirectory;

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		std::vector<std::string> PreprocessorDefinitions;

		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		std::vector<Path> SourceFiles;

		/// <summary>
		/// Gets or sets the list of library files
		/// </summary>
		std::vector<Path> LibraryFiles;

		/// <summary>
		/// Gets or sets a value indicating if the debug information should be generated
		/// </summary>
		bool GenerateSourceDebugInfo;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const CompileArguments& rhs) const
		{
			return TargetFile == rhs.TargetFile &&
				TargetType == rhs.TargetType &&
				EnableOptimize == rhs.EnableOptimize &&
				RootDirectory == rhs.RootDirectory &&
				PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
				SourceFiles == rhs.SourceFiles &&
				LibraryFiles == rhs.LibraryFiles &&
				GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo;
		}

		bool operator !=(const CompileArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
