// <copyright file="GenerateTestHost.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::Generate
{
	class GenerateTestHost : public WrenHost
	{
	private:
		static inline const char* SoupModuleName = "soup";
		static inline const char* SoupTestModuleName = "soup-test";
		static inline const char* SoupClassName = "Soup";
		static inline const char* SoupTaskClassName = "SoupTask";

	public:
		GenerateTestHost(Path scriptFile, std::optional<Path> bundlesFile) :
			WrenHost(std::move(scriptFile), std::move(bundlesFile))
		{
		}

	private:
		virtual bool IsBuiltInModule(std::string_view moduleName) override final
		{
			if (moduleName == std::string_view(SoupModuleName) ||
				moduleName == std::string_view(SoupTestModuleName))
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		virtual bool TryGetBuiltInModule(std::string_view moduleName, const char*& source) override final
		{
			// Inject Soup module
			if (moduleName == SoupModuleName)
			{
				source = GetSoupModuleSource();
				return true;
			}
			else if (moduleName == SoupTestModuleName)
			{
				source = GetSoupTestModuleSource();
				return true;
			}
			else
			{
				source = nullptr;
				return false;
			}
		}

		virtual WrenForeignMethodFn TryBindForeignMethod(
			std::string_view moduleName,
			std::string_view className,
			bool isStatic,
			std::string_view signature) override final
		{
			(moduleName);
			(className);
			(isStatic);
			(signature);
			return nullptr;
		}

		static const char* GetSoupTestModuleSource()
		{
			return soupTestModuleSource;
		}

		static const char* GetSoupModuleSource()
		{
			return soupModuleSource;
		}

		static inline const char* soupTestModuleSource =
			"class SoupTestOperation {\n"
			"	construct new(title, executable, arguments, workingDirectory, declaredInput, declaredOutput) {\n"
			"		_title = title\n"
			"		_executable = executable\n"
			"		_arguments = arguments\n"
			"		_workingDirectory = workingDirectory\n"
			"		_declaredInput = declaredInput\n"
			"		_declaredOutput = declaredOutput\n"
			"	}\n"
			"\n"
			"	title { _title }\n"
			"	executable { _executable }\n"
			"	arguments { _arguments }\n"
			"	workingDirectory { _workingDirectory }\n"
			"	declaredInput { _declaredInput }\n"
			"	declaredOutput { _declaredOutput }\n"
			"\n"
			"	==(other) {\n"
			"		return this.toString == other.toString\n"
			"	}\n"
			"\n"
			"	toString {\n"
			"		return \"SoupTestOperation { Title=%(_title), Executable=%(_executable), Arguments=%(_arguments), WorkingDirectory=%(_workingDirectory), DeclaredInput=%(_declaredInput), DeclaredOutput=%(_declaredOutput) }\"\n"
			"	}\n"
			"}\n"
			"\n"
			"class SoupTest {\n"
			"	static initialize() {\n"
			"		__globalState = {}\n"
			"		__activeState = {}\n"
			"		__sharedState = {}\n"
			"		__operations = []\n"
			"		__logs = []\n"
			"	}\n"
			"\n"
			"	static operations { __operations }\n"
			"	static logs { __logs }\n"
			"\n"
			"	static globalState {\n"
			"		if (__globalState is Null) Fiber.abort(\"Must initialize global state before access\")\n"
			"		return __globalState\n"
			"	}\n"
			"\n"
			"	static activeState {\n"
			"		if (__activeState is Null) Fiber.abort(\"Must initialize active state before access\")\n"
			"		return __activeState\n"
			"	}\n"
			"\n"
			"	static sharedState {\n"
			"		if (__sharedState is Null) Fiber.abort(\"Must initialize shared state before access\")\n"
			"		return __sharedState\n"
			"	}\n"
			"\n"
			"	static createOperation(title, executable, arguments, workingDirectory, declaredInput, declaredOutput) {\n"
			"		if (__operations is Null) Fiber.abort(\"Operations not initialized.\")\n"
			"		__operations.add(SoupTestOperation.new(title, executable, arguments, workingDirectory, declaredInput, declaredOutput))\n"
			"	}\n"
			"\n"
			"	static info(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"INFO: %(message)\")\n"
			"	}\n"
			"\n"
			"	static warning(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"WARNING: %(message)\")\n"
			"	}\n"
			"\n"
			"	static error(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"ERROR: %(message)\")\n"
			"	}\n"
			"}\n";

		static inline const char* soupModuleSource =
			"import \"soup-test\" for SoupTest\n"
			"\n"
			"class SoupTask {\n"
			"	static runBefore { [] }\n"
			"	static runAfter { [] }\n"
			"	static evaluate() {}\n"
			"}\n"
			"\n"
			"class Soup {\n"
			"	static globalState {\n"
			"		return SoupTest.globalState\n"
			"	}\n"
			"\n"
			"	static activeState {\n"
			"		return SoupTest.activeState\n"
			"	}\n"
			"\n"
			"	static sharedState {\n"
			"		return SoupTest.sharedState\n"
			"	}\n"
			"\n"
			"	static createOperation(title, executable, arguments, workingDirectory, declaredInput, declaredOutput) {\n"
			"		if (!(title is String)) Fiber.abort(\"Title must be a string.\")\n"
			"		if (!(executable is String)) Fiber.abort(\"Executable must be a string.\")\n"
			"		if (!(arguments is String)) Fiber.abort(\"Arguments must be a string.\")\n"
			"		if (!(workingDirectory is String)) Fiber.abort(\"WorkingDirectory must be a string.\")\n"
			"		if (!(declaredInput is List)) Fiber.abort(\"DeclaredInput must be a list.\")\n"
			"		if (!(declaredOutput is List)) Fiber.abort(\"DeclaredOutput must be a list.\")\n"
			"		SoupTest.createOperation(title, executable, arguments, workingDirectory, declaredInput, declaredOutput)\n"
			"	}\n"
			"\n"
			"	static info(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		SoupTest.info(message)\n"
			"	}\n"
			"\n"
			"	static warning(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		SoupTest.warning(message)\n"
			"	}\n"
			"\n"
			"	static error(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		SoupTest.error(message)\n"
			"	}\n"
			"}\n";
	};
}
