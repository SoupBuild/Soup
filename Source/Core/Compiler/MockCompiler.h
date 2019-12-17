// <copyright file="MockCompiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Mock
{
	/// <summary>
	/// A mock compiler interface implementation
	/// TODO: Move into test projects
	/// </summary>
	export class Compiler : public ICompiler
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockCompiler'/> class.
		/// </summary>
		Compiler() :
			_compileRequests(),
			_linkRequests()
		{
		}

		/// <summary>
		/// Get the compile requests
		/// </summary>
		const std::vector<CompileArguments>& GetCompileRequests() const
		{
			return _compileRequests;
		}

		/// <summary>
		/// Get the link requests
		/// </summary>
		const std::vector<LinkArguments>& GetLinkRequests() const
		{
			return _linkRequests;
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		virtual std::string_view GetName() const override final
		{
			return "MockCompiler";
		}

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		virtual std::string_view GetObjectFileExtension() const override final
		{
			return "mock.obj";
		}

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		virtual std::string_view GetModuleFileExtension() const override final
		{
			return "mock.bmi";
		}

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		virtual std::string_view GetStaticLibraryFileExtension() const override final
		{
			return "mock.lib";
		}

		/// <summary>
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		virtual std::string_view GetDynamicLibraryFileExtension() const override final
		{
			return "mock.dll";
		}

		/// <summary>
		/// Compile
		/// </summary>
		virtual CompileResult Compile(const CompileArguments& args) override final
		{
			_compileRequests.push_back(args);
			return CompileResult();
		}

		/// <summary>
		/// Link
		/// </summary>
		virtual void Link(const LinkArguments& args) override final
		{
			_linkRequests.push_back(args);
		}

	private:
		std::vector<CompileArguments> _compileRequests;
		std::vector<LinkArguments> _linkRequests;
	};
}
