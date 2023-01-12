#pragma once

#include "wren.hpp"
#include "WrenHelpers.h"

namespace Soup::Core::Generate
{
	class WrenHost
	{
	private:
		static inline const char* SoupModuleName = "soup";
		static inline const char* SoupTestModuleName = "soup-test";
		static inline const char* SoupClassName = "Soup";
		static inline const char* SoupExtensionClassName = "SoupExtension";
		Path _scriptFile;
		WrenVM* _vm;

	public:
		WrenHost(Path scriptFile) :
			_scriptFile(std::move(scriptFile)),
			_vm(nullptr)
		{
			// Configure the Wren Virtual Machine
			WrenConfiguration config;
			wrenInitConfiguration(&config);
			config.resolveModuleFn = &WrenResolveModule;
			config.loadModuleFn = &WrenLoadModule;
			config.bindForeignMethodFn = &WrenBindForeignMethod;
			config.writeFn = &WrenWriteCallback;
			config.errorFn = &WrenErrorCallback;
			config.userData = this;

			// Create the VM
			_vm = wrenNewVM(&config);
		}

		~WrenHost()
		{
			wrenFreeVM(_vm);
			_vm = nullptr;
		}

		void InterpretMain()
		{
			// Load the script
			std::ifstream scriptFile(_scriptFile.ToString());
			if (!scriptFile.is_open())
				throw std::runtime_error("Script does not exist");

			auto script = std::string(
				std::istreambuf_iterator<char>(scriptFile),
				std::istreambuf_iterator<char>());

			// Interpret the script
			WrenHelpers::ThrowIfFailed(wrenInterpret(_vm, _scriptFile.ToString().c_str(), script.c_str()));
		}

	private:
		std::vector<std::string> CallRunBeforeGetter(WrenHandle* classHandle)
		{
			// Call RunBefore
			auto runBeforeGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "runBefore"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetSlotStringList(_vm, 0, 1);
		}

		std::vector<std::string> CallRunAfterGetter(WrenHandle* classHandle)
		{
			// Call RunAfter
			auto runBeforeGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "runAfter"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetSlotStringList(_vm, 0, 1);
		}

		void WriteCallback(std::string_view text)
		{
			std::cout << text;
		}

		void ErrorCallback(
			WrenErrorType errorType,
			std::optional<std::string_view> moduleName,
			int line,
			std::string_view message)
		{
			switch (errorType)
			{
				case WREN_ERROR_COMPILE:
				{
					printf("[%s(%d)] [Error] %s\n", moduleName.value().data(), line, message.data());
					break;
				}
				case WREN_ERROR_STACK_TRACE:
				{
					printf("[%s(%d)] in %s\n", moduleName.value().data(), line, message.data());
					break;
				}
				case WREN_ERROR_RUNTIME:
				{
					printf("[Runtime Error] %s\n", message.data());
					break;
				}
			}
		}

		static WrenForeignMethodFn WrenBindForeignMethod(
			WrenVM* vm,
			const char* module,
			const char* className,
			bool isStatic,
			const char* signature)
		{
			(vm);
			(module);
			auto classNameValue = std::string_view(className);
			auto signatureValue = std::string_view(signature);

			WrenForeignMethodFn result = nullptr;

			// Attempt to bind to the Soup Module
			result = TryBindSoupModuleForeignMethod(classNameValue, isStatic, signatureValue);
			if (result != nullptr)
				return result;

			return nullptr;
		}

		static const char* ReturnRawString(const std::string& string)
		{
			char* rawString = (char*)malloc(string.size() + 1);
			memcpy(rawString, string.c_str(), string.size());
			rawString[string.size()] = '\0';
			return rawString;
		}

		static void FreeSourceOnLoaded(WrenVM* vm, const char* name, struct WrenLoadModuleResult result)
		{
			(vm);
			(name);
			if (result.source != nullptr)
				free((void*)result.source);
		}

		static const char* WrenResolveModule(
			WrenVM* vm,
			const char* importer,
			const char* moduleName)
		{
			(vm);

			// Logical import strings are used as-is and need no resolution.
			if (moduleName == std::string_view(SoupModuleName) ||
				moduleName == std::string_view(SoupTestModuleName))
				return moduleName;
			
			// Get the directory containing the importing module.
			auto modulePath = Path(importer);
			auto moduleDirectory = modulePath.GetParent();

			// Ensure the module is a relative path
			auto moduleReference = Path(moduleName);
			if (moduleReference.HasRoot())
				return moduleName;

			// Load the relative path from the current module folder
			auto resolvedModule = moduleDirectory + moduleReference;
			
			// Automatically append wren file extension
			resolvedModule.SetFileExtension("wren");

			return ReturnRawString(resolvedModule.ToString());
		}

		static WrenLoadModuleResult WrenLoadModule(WrenVM* vm, const char* module)
		{
			(vm);
			auto moduleName = std::string_view(module);

			WrenLoadModuleResult result = {0};
			result.onComplete = nullptr;

			// Inject Soup module
			if (moduleName == SoupModuleName)
			{
				result.source = GetSoupModuleSource();
			}
			else if (moduleName == SoupTestModuleName)
			{
				result.source = GetSoupTestModuleSource();
			}
			else
			{
				// Attempt to load the module as a script file
				std::ifstream scriptFile(module);
				if (scriptFile.is_open())
				{
					auto script = std::string(
						std::istreambuf_iterator<char>(scriptFile),
						std::istreambuf_iterator<char>());
						
					result.onComplete = &FreeSourceOnLoaded;
					result.source = ReturnRawString(script);
				}
			}

			return result;
		}

		static void WrenWriteCallback(WrenVM* vm, const char* text)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->WriteCallback(text);
		}

		static void WrenErrorCallback(
			WrenVM* vm,
			WrenErrorType errorType,
			const char* module,
			const int line,
			const char* msg)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			std::optional<std::string_view> moduleName = std::nullopt;
			if (module != nullptr)
				moduleName = module;
			host->ErrorCallback(errorType, moduleName, line, msg);
		}

		static const char* GetSoupTestModuleSource()
		{
			return soupTestModuleSource;
		}

		static const char* GetSoupModuleSource()
		{
			return soupModuleSource;
		}

		static WrenForeignMethodFn TryBindSoupModuleForeignMethod(
			std::string_view className,
			bool isStatic,
			std::string_view signature)
		{
			(signature);

			// There is only one foreign method in the soup module.
			if (className == SoupClassName && isStatic)
			{
			}

			return nullptr;
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
			"}\n"
			"\n"
			"class SoupTest {\n"
			"	static initialize() {\n"
			"		__globalState = null\n"
			"		__activeState = null\n"
			"		__sharedState = null\n"
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
			"	static globalState=(value) {\n"
			"		__globalState = value\n"
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
			"		__operations.add(SoupTestOperation.new(title, executable, arguments, workingDirectory, declaredInput, declaredOutput)\n"
			"	}\n"
			"\n"
			"	static debug(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"Debug: %(message)\")\n"
			"	}\n"
			"\n"
			"	static warning(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"Warning: %(message)\")\n"
			"	}\n"
			"\n"
			"	static error(message) {\n"
			"		if (__logs is Null) Fiber.abort(\"Logs not initialized.\")\n"
			"		__logs.add(\"Error: %(message)\")\n"
			"	}\n"
			"}\n";

		static inline const char* soupModuleSource =
			"import \"soup-test\" for SoupTest\n"
			"\n"
			"class SoupExtension {\n"
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
			"	static debug(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		SoupTest.debug(message)\n"
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
