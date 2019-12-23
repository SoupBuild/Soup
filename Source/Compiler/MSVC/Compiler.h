// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::MSVC
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	export class Compiler : public ICompiler
	{
	public:
		Compiler(Path toolsPath, Path compilerExecutable, Path linkerExecutable, Path libraryExecutable) :
			_toolsPath(std::move(toolsPath)),
			_compilerExecutable(std::move(compilerExecutable)),
			_linkerExecutable(std::move(linkerExecutable)),
			_libraryExecutable(std::move(libraryExecutable))
		{
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		virtual std::string_view GetName() const override final
		{
			return "MSVC";
		}

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		virtual std::string_view GetObjectFileExtension() const override final
		{
			return "obj";
		}

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		virtual std::string_view GetModuleFileExtension() const override final
		{
			return "ifc";
		}

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		virtual std::string_view GetStaticLibraryFileExtension() const override final
		{
			return "lib";
		}

		/// <summary>
		/// Gets the dynmaic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		virtual std::string_view GetDynamicLibraryFileExtension() const override final
		{
			return "dll";
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
					executablePath = _toolsPath + _libraryExecutable;
					break;
				case LinkTarget::DynamicLibrary:
				case LinkTarget::Executable:
					executablePath = _toolsPath + _linkerExecutable;
					break;
				default:
					throw std::runtime_error("Unknown LinkTarget.");
			}

			auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args);

			auto result = System::IProcessManager::Current().Execute(
				executablePath,
				commandArgs,
				args.RootDirectory);

			if (!result.StdOut.empty())
			{
				if (result.ExitCode != 0)
					Log::Warning(result.StdOut);
				else
					Log::Info(result.StdOut);
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
			auto executablePath = _toolsPath + _compilerExecutable;
			auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args, _toolsPath);

			auto result = System::IProcessManager::Current().Execute(
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
					result.StdOut,
					cleanOutput);
				result.StdOut = cleanOutput.str();
			}

			if (!result.StdOut.empty())
			{
				if (result.ExitCode != 0)
					Log::Warning(result.StdOut);
				else
					Log::Info(result.StdOut);
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
			auto executablePath = _toolsPath + _compilerExecutable;

			// Replace the final object target with the intermediate precompiled module
			auto compiledModuleArgs = CompileArguments();
			compiledModuleArgs.Standard = args.Standard;
			compiledModuleArgs.Optimize = args.Optimize;
			compiledModuleArgs.RootDirectory = args.RootDirectory;
			compiledModuleArgs.IncludeDirectories = args.IncludeDirectories;
			compiledModuleArgs.IncludeModules = args.IncludeModules;
			compiledModuleArgs.ExportModule = true;
			compiledModuleArgs.PreprocessorDefinitions = args.PreprocessorDefinitions;
			compiledModuleArgs.GenerateIncludeTree = args.GenerateIncludeTree;
			compiledModuleArgs.GenerateSourceDebugInfo = args.GenerateSourceDebugInfo;

			// Use the target file as input to the build and generate an object with the same name
			compiledModuleArgs.SourceFile = args.SourceFile;
			compiledModuleArgs.TargetFile = args.TargetFile;

			auto compiledModuleCommandArgs =
				ArgumentBuilder::BuildCompilerArguments(compiledModuleArgs, _toolsPath);
			auto result = System::IProcessManager::Current().Execute(
				executablePath,
				compiledModuleCommandArgs,
				args.RootDirectory);

			// Pull out the include paths if requested
			auto compileResult = CompileResult();
			if (compiledModuleArgs.GenerateIncludeTree)
			{
				std::stringstream cleanOutput;
				compileResult.HeaderIncludeFiles = ParseIncludes(
					compiledModuleArgs.SourceFile,
					result.StdOut,
					cleanOutput);
				result.StdOut = cleanOutput.str();
			}

			if (!result.StdOut.empty())
			{
				if (result.ExitCode != 0)
					Log::Warning(result.StdOut);
				else
					Log::Info(result.StdOut);
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
					auto includePrefix = GetIncludePrefix();
					auto offset = includePrefix.size() + includeDepth;
					auto includeFile = Path(line.substr(offset));

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
			auto includePrefix = GetIncludePrefix();
			if (line.rfind(includePrefix, 0) == 0)
			{
				// Find the end of the whitespace
				int offset = includePrefix.size();
				for (; offset < line.size(); offset++)
				{
					if (line[offset] != ' ')
					{
						break;
					}
				}

				// The depth is the number of whitespaces past the prefix
				depth = offset - includePrefix.size();
			}

			return depth;
		}

		std::string_view GetIncludePrefix()
		{
			return std::string_view("Note: including file:");
		}

	private:
		Path _toolsPath;
		Path _compilerExecutable;
		Path _linkerExecutable;
		Path _libraryExecutable;
	};
}
