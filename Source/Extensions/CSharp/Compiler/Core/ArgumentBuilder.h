// <copyright file="ArgumentBuilder.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "CompileArguments.h"

namespace Soup::CSharp::Compiler
{
	/// <summary>
	/// A helper class that builds the correct set of compiler arguments for a given
	/// set of options.
	/// </summary>
	export class ArgumentBuilder
	{
	public:
		static std::vector<std::string> BuildCompilerArguments(
			const CompileArguments& arguments)
		{
			// Calculate object output file
			auto commandArgs = std::vector<std::string>();

			// Do not auto include CSC.RSP file
			AddFlag(commandArgs, "noconfig");

			// Disable 'unsafe' code
			AddFlag(commandArgs, "unsafe-");

			// Disable generate overflow checks
			AddFlag(commandArgs, "checked-");

			// TODO /nowarn:1701,1702,1701,1702,2008

			// Tell compiler to generate fully qualified paths
			AddFlag(commandArgs, "fullpaths");

			// Do not reference standard library mscorlib.dll
			AddFlag(commandArgs, "nostdlib+");

			// Specify how to handle internal compiler errors: prompt, send, queue, or none. The default is queue.
			AddParameter(commandArgs, "errorreport", "prompt");

			// Set warning level (0-4)
			AddParameter(commandArgs, "warn", "4");

			// Set the condition compilation definitions
			if (!arguments.PreprocessorDefinitions.empty())
			{
				AddParameterList(commandArgs, "define", arguments.PreprocessorDefinitions);
			}

			// Output line and column of the end location of each error
			AddFlag(commandArgs, "errorendlocation");

			// Specify the preferred output language name.
			AddParameter(commandArgs, "preferreduilang", "en-US");

			// Enable high-entropy ASLR
			AddFlag(commandArgs, "highentropyva+");

			// Add the library references
			for (auto& file : arguments.LibraryFiles)
			{
				// Add the library files as reference
				AddParameterWithQuotes(commandArgs, "reference", file.ToString());
			}

			// Emit debugging information
			if (arguments.GenerateSourceDebugInfo)
			{
				AddFlag(commandArgs, "debug+");
			}
			else
			{
				AddFlag(commandArgs, "debug-");
			}

			// Emit debugging information to to .pdb file using cross-platform Portable PDB format
			AddParameter(commandArgs, "debug", "portable");

			// Specify the alignment used for output file sections
			AddParameter(commandArgs, "filealign", "512");

			// Disable optimizations
			if (arguments.EnableOptimize)
			{
				AddFlag(commandArgs, "optimize+");
			}
			else
			{
				AddFlag(commandArgs, "optimize-");
			}

			// Specify output file name
			AddParameter(commandArgs, "out", arguments.TargetFile.ToString());

			// Specify a ruleset file that disables specific diagnostics.
			// TODO: /ruleset:"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Team Tools\Static Analysis Tools\\Rule Sets\MinimumRecommendedRules.ruleset"

			// /target:exe
			switch (arguments.TargetType)
			{
			case TargetType::Library:
				AddParameter(commandArgs, "target", "library");
				break;
			case TargetType::Executable:
				AddParameter(commandArgs, "target", "exe");
				break;
			default:
				throw std::runtime_error("Unknown TargetType");
			}

			// Report all warnings as errors
			AddFlag(commandArgs, "warnaserror+");

			// Output compiler messages in UTF-8 encoding
			AddFlag(commandArgs, "utf8output");

			// Produce a deterministic assembly (including module version GUID and timestamp)
			AddFlag(commandArgs, "deterministic+");

			// Specify language version such as default (latest major version), or latest (latest version, including minor versions)
			AddParameter(commandArgs, "langversion", "8.0");

			// Add the source files
			for (auto& file : arguments.SourceFiles)
			{
				// Add the files as input
				commandArgs.push_back(file.ToString());
			}

			// TODO: /warnaserror+:NU1605

			return commandArgs;
		}

	private:
		static void AddValueWithQuotes(
			std::vector<std::string>& args,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "\"" << value << "\"";
			args.push_back(builder.str());
		}

		static void AddFlag(std::vector<std::string>& args, std::string_view flag)
		{
			auto builder = std::stringstream();
			builder << "/" << flag;
			args.push_back(builder.str());
		}

		static void AddFlagValue(
			std::vector<std::string>& args,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << value;
			args.push_back(builder.str());
		}

		static void AddFlagValueWithQuotes(
			std::vector<std::string>& args,
			std::string_view flag,
			std::string value)
		{
			auto builder = std::stringstream();
			builder << "/" << flag << "\"" << value << "\"";
			args.push_back(builder.str());
		}

		static void AddParameter(
			std::vector<std::string>& args,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":" << value;
			args.push_back(builder.str());
		}

		static void AddParameterList(
			std::vector<std::string>& args,
			std::string_view name,
			const std::vector<std::string>& values)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":";
			bool isFirst = true;
			for (auto& value : values)
			{
				if (!isFirst)
				{
					builder << ";";
				}

				builder << value;
				isFirst = false;
			}

			args.push_back(builder.str());
		}

		static void AddParameterWithQuotes(
			std::vector<std::string>& args,
			std::string_view name,
			std::string_view value)
		{
			auto builder = std::stringstream();
			builder << "/" << name << ":\"" << value << "\"";
			args.push_back(builder.str());
		}
	};
}
