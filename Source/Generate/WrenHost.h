#pragma once

#include "wren.hpp"
#include "WrenHelpers.h"
#include "ExtensionDetails.h"

namespace Soup::Core::Generate
{
	class WrenHost
	{
	private:
		static inline const char* MainModuleName = "main";
		static inline const char* SoupModuleName = "soup";
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
			auto script = std::string(
				std::istreambuf_iterator<char>(scriptFile),
				std::istreambuf_iterator<char>());

			// Interpret the script
			WrenHelpers::ThrowIfFailed(wrenInterpret(_vm, MainModuleName, script.c_str()));
		}

		std::vector<ExtensionDetails> DiscoverExtensions()
		{
			auto extensions = std::vector<ExtensionDetails>();

			// Discover all class types
			wrenEnsureSlots(_vm, 1);
			auto variableCount = wrenGetVariableCount(_vm, MainModuleName);
			for (auto i = 0; i < variableCount; i++)
			{
				wrenGetVariableAt(_vm, MainModuleName, i, 0);

				// Check if a class
				auto type = wrenGetSlotType(_vm, 0);
				if (type == WREN_TYPE_UNKNOWN)
				{
					auto classHandle = WrenHelpers::SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));
					if (WrenHelpers::HasParentType(_vm, classHandle, SoupExtensionClassName))
					{
						Log::Diag("Found Build Extension");
						auto className = WrenHelpers::GetClassName(_vm, classHandle);
						auto runBeforeList = CallRunBeforeGetter(classHandle);
						auto runAfterList = CallRunAfterGetter(classHandle);

						extensions.push_back(
							ExtensionDetails(
								std::move(className),
								_scriptFile,
								std::move(runBeforeList),
								std::move(runAfterList)));
					}
				}
			}

			return extensions;
		}

		void EvaluateExtension(const std::string& className)
		{
			// Load up the class
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, MainModuleName, className.c_str(), 0);

			// Check if a class
			auto type = wrenGetSlotType(_vm, 0);
			if (type != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Extension class name was not a class");
			}

			auto classHandle = WrenHelpers::SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call Evaluate
			auto evaluateMethodHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "evaluate()"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, evaluateMethodHandle));
		}

		void LoadActiveState()
		{
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, SoupModuleName, SoupClassName, 0);

			auto soupClassType = wrenGetSlotType(_vm, 0);
			if (soupClassType != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Missing Class Soup");
			}

			auto soupClassHandle = WrenHelpers::SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call ActiveState
			auto activeStateGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "activeState"));
			wrenSetSlotHandle(_vm, 0, soupClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, activeStateGetterHandle));

			auto activeStateType = wrenGetSlotType(_vm, 0);
			if (activeStateType != WREN_TYPE_MAP) {
				throw std::runtime_error("ActiveState must be a map");
			}

			wrenEnsureSlots(_vm, 3);
			auto mapCount = wrenGetMapCount(_vm, 0);
			Log::Diag("ActiveState: ");
			for (auto i = 0; i < mapCount; i++)
			{
				wrenGetMapKeyValueAt(_vm, 0, i, 1, 2);
				auto key = wrenGetSlotString(_vm, 1);
				auto value = wrenGetSlotString(_vm, 2);
				Log::Diag(key);
				(value);
			}
		}

	private:
		std::vector<std::string> CallRunBeforeGetter(WrenHandle* classHandle)
		{
			// Call RunBefore
			auto runBeforeGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "runBefore"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetResultAsStringList(_vm);
		}

		std::vector<std::string> CallRunAfterGetter(WrenHandle* classHandle)
		{
			// Call RunAfter
			auto runBeforeGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "runAfter"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetResultAsStringList(_vm);
		}

		void WriteCallback(std::string_view text)
		{
			Log::Diag(text);
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
					printf("[%s line %d] [Error] %s\n", moduleName.value().data(), line, message.data());
					break;
				}
				case WREN_ERROR_STACK_TRACE:
				{
					printf("[%s line %d] in %s\n", moduleName.value().data(), line, message.data());
					break;
				}
				case WREN_ERROR_RUNTIME:
				{
					printf("[Runtime Error] %s\n", message.data());
					break;
				}
			}
		}

		void SoupLoadActiveState()
		{
			Log::Diag("SoupLoadActiveState");
			wrenSetSlotNewMap(_vm, 0);
		}

		void SoupLoadSharedState()
		{
			Log::Diag("SoupLoadSharedState");
			wrenSetSlotNewMap(_vm, 0);
		}

		void SoupCreateOperation()
		{
			Log::Diag("SoupCreateOperation");
			wrenSetSlotNull(_vm, 0);
		}

		void SoupLogDebug()
		{
			auto message = wrenGetSlotString(_vm, 1);
			Log::Info(message);
			wrenSetSlotNull(_vm, 0);
		}

		void SoupLogWarning()
		{
			auto message = wrenGetSlotString(_vm, 1);
			Log::Warning(message);
			wrenSetSlotNull(_vm, 0);
		}

		void SoupLogError()
		{
			auto message = wrenGetSlotString(_vm, 1);
			Log::Error(message);
			wrenSetSlotNull(_vm, 0);
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

		static WrenLoadModuleResult WrenLoadModule(WrenVM* vm, const char* module)
		{
			(vm);
			auto moduleName = std::string_view(module);

			WrenLoadModuleResult result = {0};
			result.onComplete = nullptr;

			// Inject Soup module
			if (moduleName == SoupModuleName)
				result.source = GetSoupModuleSource();

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

		static void SoupLoadActiveState(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupLoadActiveState();
		}

		static void SoupLoadSharedState(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupLoadSharedState();
		}

		static void SoupCreateOperation(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupCreateOperation();
		}

		static void SoupLogDebug(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupLogDebug();
		}

		static void SoupLogWarning(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupLogWarning();
		}

		static void SoupLogError(WrenVM* vm)
		{
			auto host = (WrenHost*)wrenGetUserData(vm);
			host->SoupLogError();
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
			// There is only one foreign method in the soup module.
			if (className == SoupClassName && isStatic)
			{
				if (signature == "loadActiveState_()")
					return SoupLoadActiveState;
				else if (signature == "loadSharedState_()")
					return SoupLoadSharedState;
				else if (signature == "createOperation_(_,_,_,_,_,_)")
					return SoupCreateOperation;
				else if (signature == "debug_(_)")
					return SoupLogDebug;
				else if (signature == "warning_(_)")
					return SoupLogWarning;
				else if (signature == "error_(_)")
					return SoupLogError;
			}

			return nullptr;
		}

		static inline const char* soupModuleSource =
			"class SoupExtension {\n"
			"	static runBefore { [] }\n"
			"	static runAfter { [] }\n"
			"	static evaluate() {}\n"
			"}\n"
			"\n"
			"class Soup {\n"
			"	static activeState {\n"
			"		if (__activeState is Null) __activeState = loadActiveState_()\n"
			"		return __activeState\n"
			"	}\n"
			"\n"
			"	static sharedState {\n"
			"		if (__sharedState is Null) __sharedState = loadSharedState_()\n"
			"		return __sharedState\n"
			"	}\n"
			"\n"
			"	static createOperation(title, executable, arguments, workingDirectory, declaredInput, declaredOutput) {\n"
			"		if (!(title is String)) Fiber.abort(\"Title must be a string.\")\n"
			"		if (!(executable is String)) Fiber.abort(\"Executable must be a string.\")\n"
			"		if (!(arguments is String)) Fiber.abort(\"Arguments must be a string.\")\n"
			"		if (!(workingDirectory is String)) Fiber.abort(\"WorkingDirectory must be a string.\")\n"
			"		if (!(declaredInput is String)) Fiber.abort(\"DeclaredInput must be a string.\")\n"
			"		if (!(declaredOutput is String)) Fiber.abort(\"DeclaredOutput must be a string.\")\n"
			"		createOperation_(title, executable, arguments, workingDirectory, declaredInput, declaredOutput)\n"
			"	}\n"
			"\n"
			"	static debug(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		debug_(message)\n"
			"	}\n"
			"\n"
			"	static warning(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		warning_(message)\n"
			"	}\n"
			"\n"
			"	static error(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		error_(message)\n"
			"	}\n"
			"\n"
			"	foreign static loadActiveState_()\n"
			"	foreign static loadSharedState_()\n"
			"	foreign static createOperation_(title, executable, arguments, workingDirectory, declaredInput, declaredOutput)\n"
			"	foreign static debug_(message)\n"
			"	foreign static warning_(message)\n"
			"	foreign static error_(message)\n"
			"}\n";
	};
}
