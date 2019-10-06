// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::Clang
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	export class Compiler : public ICompiler
	{
	private:
		// static Path ToolsPath = "C:/Program Files/llvm/";
		static constexpr std::string_view ToolsPath = "C:/Clang";
		static constexpr std::string_view CompilerExecutable = "bin/clang++.exe";
		static constexpr std::string_view LinkerExecutable = "bin/llvm-ar.exe";

	public:
		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		virtual std::string_view GetName() const override final
		{
			return "Clang";
		}

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		virtual std::string_view GetObjectFileExtension() const override final
		{
			return "o";
		}

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		virtual std::string_view GetModuleFileExtension() const override final
		{
			return "pcm";
		}

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		virtual std::string_view GetStaticLibraryFileExtension() const override final
		{
			return "a";
		}

		/// <summary>
		/// Compile
		/// </summary>
		virtual CompileResult Compile(const CompileArguments& args) override final
		{
			// Clang decided to do their module compilation in two stages
			// Now we have to also generate the object file from the precompiled module
			if (args.ExportModule)
			{
				return CompileModuleInterfaceUnit(args);
			}
			else
			{
				return CompileStandard(args);
			}
		}

		/// <summary>
		/// Link
		/// </summary>
		virtual void Link(const LinkArguments& args) override final
		{
			// Select the correct executable for linking libraries or executables
			Path executablePath;
			switch (args.TargetType)
			{
				case LinkTarget::StaticLibrary:
					executablePath = Path(ToolsPath) + Path(LinkerExecutable);
					break;
				case LinkTarget::Executable:
					executablePath = Path(ToolsPath) + Path(CompilerExecutable);
					break;
				default:
					throw std::runtime_error("Unknown LinkTarget.");
			}

			auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args);

			auto result = IProcessManager::Current().Execute(
				executablePath,
				commandArgs,
				args.RootDirectory);

			if (!result.StdOut.empty())
			{
				Log::Verbose(result.StdOut);
			}

			// If there was any error output then the build failed
			if (!result.StdErr.empty())
			{
				Log::Warning(result.StdErr);
			}
			
			if (result.ExitCode != 0)
			{
				throw std::runtime_error("Linker Error: " + std::to_string(result.ExitCode));
			}
		}

	private:
		CompileResult CompileStandard(const CompileArguments& args)
		{
			auto executablePath = Path(ToolsPath) + Path(CompilerExecutable);
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

			auto result = IProcessManager::Current().Execute(
				executablePath,
				commandArgs,
				args.RootDirectory);

			// Pull out the include paths if requested
			auto compileResult = CompileResult();
			if (args.GenerateIncludeTree)
			{
				std::stringstream cleanOutput;
				compileResult.HeaderIncludeFiles = ParseIncludes(
					args.SourceFile,
					result.StdErr,
					cleanOutput);
				result.StdErr = cleanOutput.str();
			}

			if (!result.StdOut.empty())
			{
				Log::Verbose(result.StdOut);
			}

			// If there was any error output then the build failed
			// TODO: Find warnings + errors
			if (!result.StdErr.empty())
			{
				Log::Warning(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				Log::Error("Compile standard failed");
				throw std::runtime_error("Compile standard failed");
			}

			return compileResult;
		}

		CompileResult CompileModuleInterfaceUnit(const CompileArguments& args)
		{
			auto executablePath = Path(ToolsPath) + Path(CompilerExecutable);

			// Replace the final object target with the intermediate precompiled module
			auto generatePrecompiledModuleArgs = CompileArguments();
			generatePrecompiledModuleArgs.Standard = args.Standard;
			generatePrecompiledModuleArgs.Optimize = args.Optimize;
			generatePrecompiledModuleArgs.RootDirectory = args.RootDirectory;
			generatePrecompiledModuleArgs.IncludeDirectories = args.IncludeDirectories;
			generatePrecompiledModuleArgs.IncludeModules = args.IncludeModules;
			generatePrecompiledModuleArgs.ExportModule = true;
			generatePrecompiledModuleArgs.PreprocessorDefinitions = args.PreprocessorDefinitions;
			generatePrecompiledModuleArgs.GenerateIncludeTree = args.GenerateIncludeTree;

			// Use the target file as input to the build and generate an object with the same name
			generatePrecompiledModuleArgs.SourceFile = args.SourceFile;
			generatePrecompiledModuleArgs.TargetFile = args.TargetFile;
			generatePrecompiledModuleArgs.TargetFile.SetFileExtension(GetModuleFileExtension());

			auto generatePrecompiledModuleCommandArgs = ArgumentBuilder::BuildCompilerArguments(generatePrecompiledModuleArgs);
			auto result = IProcessManager::Current().Execute(
				executablePath,
				generatePrecompiledModuleCommandArgs,
				args.RootDirectory);

			// Pull out the include paths if requested
			auto compileResult = CompileResult();
			if (generatePrecompiledModuleArgs.GenerateIncludeTree)
			{
				std::stringstream cleanOutput;
				compileResult.HeaderIncludeFiles = ParseIncludes(
					generatePrecompiledModuleArgs.SourceFile,
					result.StdErr,
					cleanOutput);
				result.StdErr = cleanOutput.str();
			}

			if (!result.StdOut.empty())
			{
				Log::Verbose(result.StdOut);
			}

			// If there was any error output then the build failed
			// TODO: Find warnings + errors
			if (!result.StdErr.empty())
			{
				Log::Warning(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				Log::Error("Compile module interface failed");
				throw std::runtime_error("Compiler Precompile Error: " + std::to_string(result.ExitCode));
			}

			// Now we can compile the object file from the precompiled module
			auto compileObjectArgs = CompileArguments();
			compileObjectArgs.Standard = args.Standard;
			compileObjectArgs.Optimize = args.Optimize;
			compileObjectArgs.RootDirectory = args.RootDirectory;
			compileObjectArgs.SourceFile = generatePrecompiledModuleArgs.TargetFile;
			compileObjectArgs.TargetFile = args.TargetFile;

			auto compileObjectCommandArgs = ArgumentBuilder::BuildCompilerArguments(compileObjectArgs);
			result = IProcessManager::Current().Execute(
				executablePath,
				compileObjectCommandArgs,
				args.RootDirectory);

			if (!result.StdOut.empty())
			{
				Log::Verbose(result.StdOut);
			}

			// If there was any error output then the build failed
			// TODO: Find warnings + errors
			if (!result.StdErr.empty())
			{
				Log::Warning(result.StdErr);
			}

			if (result.ExitCode != 0)
			{
				throw std::runtime_error("Compiler Object Error: " + std::to_string(result.ExitCode));
			}

			return compileResult;
		}

		std::vector<HeaderInclude> ParseIncludes(
			const Path& file,
			const std::string& output,
			std::stringstream& cleanOutput)
		{
			// Add the root file
			std::stack<HeaderInclude> current;
			current.push(HeaderInclude(file));

			std::stringstream content(output);
			std::string line;
			while (std::getline(content, line))
			{
				// TODO: Getline is dumb and uses newline on windows
				if (line[line.size() - 1] == '\r')
				{
					line.resize(line.size() - 1);
				}

				auto includeDepth = GetIncludeDepth(line);
				if (includeDepth > 0)
				{
					// Parse the file reference
					auto includeFile = Path(line.substr(includeDepth + 1));

					// Ensure we are at the correct depth
					while (includeDepth < current.size())
					{
						// Remove the top file and push it onto its parent
						auto previous = std::move(current.top());
						current.pop();
						current.top().Includes.push_back(std::move(previous));
					}

					// Ensure we do not try to go up more than one level at a time
					if (includeDepth > current.size() + 1)
						throw std::runtime_error("Missing an include level.");

					current.push(HeaderInclude(includeFile));
				}
				else
				{
					// Not an include, pass along
					cleanOutput << line << "\n";
				}
			}

			// Ensure we are at the top level
			while (1 < current.size())
			{
				// Remove the top file and push it onto its parent
				auto previous = std::move(current.top());
				current.pop();
				current.top().Includes.push_back(std::move(previous));
			}

			return std::vector<HeaderInclude>({ std::move(current.top()) });
		}

		int GetIncludeDepth(const std::string& line)
		{
			int depth = 0;
			for (depth = 0; depth < line.size(); depth++)
			{
				if (line[depth] != '.')
				{
					break;
				}
			}

			// Verify the next character is a space, otherwise reset the depth to zero
			if (depth < line.size() && line[depth] != ' ')
			{
				depth = 0;
			}

			return depth;
		}
	};
}
