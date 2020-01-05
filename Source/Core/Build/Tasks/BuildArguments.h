// <copyright file="BuildArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "CompileArguments.h"

namespace Soup
{
	/// <summary>
	/// The enumeration of build optimization levels
	/// </summary>
	export enum class BuildOptimizationLevel
	{
		/// <summary>
		/// Debug
		/// </summary>
		None,

		/// <summary>
		/// Optimize for runtime speed, may sacrifice size
		/// </summary>
		Speed,

		/// <summary>
		/// Optimize for speed and size
		/// </summary>
		Size,
	};

	std::string ToString(BuildOptimizationLevel value)
	{
		switch (value)
		{
			case BuildOptimizationLevel::None:
				return "None";
			case BuildOptimizationLevel::Speed:
				return "Speed";
			case BuildOptimizationLevel::Size:
				return "Size";
			default:
				throw std::runtime_error("Unknown BuildOptimizationLevel");
		}
	}

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
		/// Static Library
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// Dynamic Library
		/// </summary>
		DynamicLibrary,
	};

	std::string ToString(BuildTargetType value)
	{
		switch (value)
		{
			case BuildTargetType::Executable:
				return "Executable";
			case BuildTargetType::StaticLibrary:
				return "StaticLibrary";
			case BuildTargetType::DynamicLibrary:
				return "DynamicLibrary";
			default:
				throw std::runtime_error("Unknown BuildTargetType");
		}
	}

	/// <summary>
	/// The source file definition
	/// </summary>
	export struct SourceFile
	{
	public:
		Path File;
		bool IsModule;
	};

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
		/// Gets or sets the target type
		/// </summary>
		BuildTargetType TargetType;

		/// <summary>
		/// Gets or sets the language standard
		/// </summary>
		LanguageStandard LanguageStandard;

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
		/// Gets or sets the single module interface source file
		/// </summary>
		Path ModuleInterfaceSourceFile;

		/// <summary>
		/// Gets or sets the list of source files
		/// Note: These files can be plain old translation units 
		/// or they can be module implementation units
		/// </summary>
		std::vector<Path> SourceFiles;

		/// <summary>
		/// Gets or sets the list of include directories
		/// </summary>
		std::vector<Path> IncludeDirectories;

		/// <summary>
		/// Gets or sets the list of link libraries
		/// </summary>
		std::vector<Path> LinkLibraries;

		/// <summary>
		/// Gets or sets the list of library paths
		/// </summary>
		std::vector<Path> LibraryPaths;

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		std::vector<std::string> PreprocessorDefinitions;

		/// <summary>
		/// Gets or sets the optimization level
		/// </summary>
		BuildOptimizationLevel OptimizationLevel;

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
				TargetType == rhs.TargetType &&
				LanguageStandard == rhs.LanguageStandard &&
				WorkingDirectory == rhs.WorkingDirectory &&
				ObjectDirectory == rhs.ObjectDirectory &&
				BinaryDirectory == rhs.BinaryDirectory &&
				ModuleInterfaceSourceFile == rhs.ModuleInterfaceSourceFile &&
				SourceFiles == rhs.SourceFiles &&
				IncludeDirectories == rhs.IncludeDirectories &&
				LinkLibraries == rhs.LinkLibraries &&
				LibraryPaths == rhs.LibraryPaths &&
				PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
				OptimizationLevel == rhs.OptimizationLevel &&
				GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo;
		}

		bool operator !=(const BuildArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
