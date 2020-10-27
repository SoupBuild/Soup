// <copyright file="CompilerArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler
{
	/// <summary>
	/// The enumeration of language standards
	/// </summary>
	export enum class LanguageStandard
	{
		/// <summary>
		/// C++ 11
		/// </summary>
		CPP11,

		/// <summary>
		/// C++ 14
		/// </summary>
		CPP14,

		/// <summary>
		/// C++ 17
		/// </summary>
		CPP17,

		/// <summary>
		/// C++ 20
		/// </summary>
		CPP20,
	};

	std::string ToString(LanguageStandard value)
	{
		switch (value)
		{
			case LanguageStandard::CPP11:
				return "C++11";
			case LanguageStandard::CPP14:
				return "C++14";
			case LanguageStandard::CPP17:
				return "C++17";
			case LanguageStandard::CPP20:
				return "C++20";
			default:
				throw std::runtime_error("Unknown LanguageStandard");
		}
	}

	/// <summary>
	/// The enumeration of optimization levels
	/// </summary>
	export enum class OptimizationLevel
	{
		/// <summary>
		/// Disable all optimization for build speed and debugability
		/// </summary>
		None,

		/// <summary>
		/// Optimize for speed
		/// </summary>
		Speed,

		/// <summary>
		/// Optimize for size
		/// </summary>
		Size,
	};

	std::string ToString(OptimizationLevel value)
	{
		switch (value)
		{
			case OptimizationLevel::None:
				return "None";
			case OptimizationLevel::Speed:
				return "Speed";
			case OptimizationLevel::Size:
				return "Size";
			default:
				throw std::runtime_error("Unknown OptimizationLevel");
		}
	}

	/// <summary>
	/// The set of file specific compiler arguments
	/// </summary>
	export struct TranslationUnitCompileArguments
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		Path SourceFile;

		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		Path TargetFile;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const TranslationUnitCompileArguments& rhs) const
		{
			return SourceFile == rhs.SourceFile &&
				TargetFile == rhs.TargetFile;
		}

		bool operator !=(const TranslationUnitCompileArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};

	/// <summary>
	/// The set of file specific compiler arguments
	/// </summary>
	export struct InterfaceUnitCompileArguments : public TranslationUnitCompileArguments
	{
		/// <summary>
		/// Gets or sets the source file
		/// </summary>
		Path ModuleInterfaceTarget;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const InterfaceUnitCompileArguments& rhs) const
		{
			return SourceFile == rhs.SourceFile &&
				TargetFile == rhs.TargetFile &&
				ModuleInterfaceTarget == rhs.ModuleInterfaceTarget;
		}

		bool operator !=(const InterfaceUnitCompileArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};

	/// <summary>
	/// The set of shared compiler arguments
	/// </summary>
	export struct SharedCompileArguments
	{
		/// <summary>
		/// Gets or sets the language standard
		/// </summary>
		LanguageStandard Standard;

		/// <summary>
		/// Gets or sets the optimization level
		/// </summary>
		OptimizationLevel Optimize;

		/// <summary>
		/// Gets or sets the root directory
		/// </summary>
		Path RootDirectory;

		/// <summary>
		/// Gets or sets the object directory
		/// </summary>
		Path ObjectDirectory;

		/// <summary>
		/// Gets or sets the list of preprocessor definitions
		/// </summary>
		std::vector<std::string> PreprocessorDefinitions;

		/// <summary>
		/// Gets or sets the list of include directories
		/// </summary>
		std::vector<Path> IncludeDirectories;

		/// <summary>
		/// Gets or sets the list of modules
		/// </summary>
		std::vector<Path> IncludeModules;

		/// <summary>
		/// Gets or sets a value indicating whether to generate source debug information
		/// </summary>
		bool GenerateSourceDebugInfo;

		/// <summary>
		/// Gets or sets the single optional interface unit to compile
		/// </summary>
		std::optional<InterfaceUnitCompileArguments> InterfaceUnit;

		/// <summary>
		/// Gets or sets the list of individual translation units to compile
		/// </summary>
		std::vector<TranslationUnitCompileArguments> ImplementationUnits;

		/// <summary>
		/// Gets or sets a value indicating whether to enable warnings as errors
		/// </summary>
		bool EnableWarningsAsErrors;

		/// <summary>
		/// Gets or sets the list of disabled warnings
		/// </summary>
		std::vector<std::string> DisabledWarnings;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const SharedCompileArguments& rhs) const
		{
			return Standard == rhs.Standard &&
				Optimize == rhs.Optimize &&
				RootDirectory == rhs.RootDirectory &&
				PreprocessorDefinitions == rhs.PreprocessorDefinitions &&
				IncludeDirectories == rhs.IncludeDirectories &&
				IncludeModules == rhs.IncludeModules &&
				GenerateSourceDebugInfo == rhs.GenerateSourceDebugInfo &&
				InterfaceUnit == rhs.InterfaceUnit &&
				ImplementationUnits == rhs.ImplementationUnits &&
				EnableWarningsAsErrors == rhs.EnableWarningsAsErrors &&
				DisabledWarnings == rhs.DisabledWarnings;
		}

		bool operator !=(const SharedCompileArguments& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
