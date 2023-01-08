#pragma once

#include "wren.hpp"
#include "WrenHelpers.h"
#include "WrenValueTable.h"
#include "ExtensionDetails.h"
#include "GenerateState.h"

namespace Soup::Core::Generate
{
	class WrenHost
	{
	private:
		static inline const char* SoupModuleName = "soup";
		static inline const char* SoupClassName = "Soup";
		static inline const char* SoupExtensionClassName = "SoupExtension";
		Path _scriptFile;
		WrenVM* _vm;
		GenerateState* _state;

	public:
		WrenHost(Path scriptFile) :
			_scriptFile(std::move(scriptFile)),
			_vm(nullptr),
			_state(nullptr)
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

		void SetState(GenerateState& state)
		{
			_state = &state;
		}

		void InterpretMain()
		{
			Log::Diag("InterpretMain");

			// Load the script
			std::ifstream scriptFile(_scriptFile.ToString());
			if (!scriptFile.is_open())
				throw std::runtime_error("Script does not exist");

			auto script = std::string(
				std::istreambuf_iterator<char>(scriptFile),
				std::istreambuf_iterator<char>());

			// Interpret the script
			WrenHelpers::ThrowIfFailed(wrenInterpret(_vm, _scriptFile.ToString().c_str(), script.c_str()));
			Log::Diag("Success");
		}

		std::vector<ExtensionDetails> DiscoverExtensions()
		{
			auto extensions = std::vector<ExtensionDetails>();

			// Discover all class types
			wrenEnsureSlots(_vm, 1);
			auto variableCount = wrenGetVariableCount(_vm, _scriptFile.ToString().c_str());
			for (auto i = 0; i < variableCount; i++)
			{
				wrenGetVariableAt(_vm, _scriptFile.ToString().c_str(), i, 0);

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
			wrenGetVariable(_vm, _scriptFile.ToString().c_str(), className.c_str(), 0);

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

		ValueTable LoadActiveState()
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

			auto result = WrenValueTable::GetSlotTable(_vm, 0);
			return result;
		}

		ValueTable LoadSharedState()
		{
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, SoupModuleName, SoupClassName, 0);

			auto soupClassType = wrenGetSlotType(_vm, 0);
			if (soupClassType != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Missing Class Soup");
			}

			auto soupClassHandle = WrenHelpers::SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call ActiveState
			auto activeStateGetterHandle = WrenHelpers::SmartHandle(_vm, wrenMakeCallHandle(_vm, "sharedState"));
			wrenSetSlotHandle(_vm, 0, soupClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, activeStateGetterHandle));

			auto result = WrenValueTable::GetSlotTable(_vm, 0);
			return result;
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

		void SoupLoadActiveState()
		{
			try
			{
				Log::Diag("SoupLoadActiveState");
				if (_state == nullptr)
					throw std::runtime_error("Cannot load ActiveState at this time");

				WrenValueTable::SetSlotTable(_vm, 0, _state->ActiveState());
			}
			catch(const std::exception& ex)
			{
				WrenHelpers::GenerateRuntimeError(_vm, ex.what());
			}
		}

		void SoupLoadSharedState()
		{
			try
			{
				Log::Diag("SoupLoadSharedState");
				if (_state == nullptr)
					throw std::runtime_error("Cannot load SharedState at this time");

				WrenValueTable::SetSlotTable(_vm, 0, _state->SharedState());
			}
			catch(const std::exception& ex)
			{
				WrenHelpers::GenerateRuntimeError(_vm, ex.what());
			}
		}

		void SoupCreateOperation()
		{
			try
			{
				Log::Diag("SoupCreateOperation");
				if (_state == nullptr)
					throw std::runtime_error("Cannot CreateOperation at this time");

				auto parameter1 = wrenGetSlotType(_vm, 1);
				if (parameter1 != WREN_TYPE_STRING) {
					throw std::runtime_error("SoupCreateOperation parameter 1 must be of type string");
				}
				auto title = std::string(wrenGetSlotString(_vm, 1));

				auto parameter2 = wrenGetSlotType(_vm, 2);
				if (parameter2 != WREN_TYPE_STRING) {
					throw std::runtime_error("SoupCreateOperation parameter 2 must be of type string");
				}
				auto executable = std::string(wrenGetSlotString(_vm, 2));

				auto parameter3 = wrenGetSlotType(_vm, 3);
				if (parameter3 != WREN_TYPE_STRING) {
					throw std::runtime_error("SoupCreateOperation parameter 3 must be of type string");
				}
				auto arguments = std::string(wrenGetSlotString(_vm, 3));
				
				auto parameter4 = wrenGetSlotType(_vm, 4);
				if (parameter4 != WREN_TYPE_STRING) {
					throw std::runtime_error("SoupCreateOperation parameter 4 must be of type string");
				}
				auto workingDirectory = std::string(wrenGetSlotString(_vm, 4));

				auto parameter5 = wrenGetSlotType(_vm, 5);
				if (parameter5 != WREN_TYPE_LIST) {
					throw std::runtime_error("SoupCreateOperation parameter 5 must be of type list");
				}
				auto declaredInput = WrenHelpers::GetSlotStringList(_vm, 5, 7);

				auto parameter6 = wrenGetSlotType(_vm, 6);
				if (parameter6 != WREN_TYPE_LIST) {
					throw std::runtime_error("SoupCreateOperation parameter 6 must be of type list");
				}
				auto declaredOutput = WrenHelpers::GetSlotStringList(_vm, 6, 7);

				_state->CreateOperation(
					std::move(title),
					std::move(executable),
					std::move(arguments),
					std::move(workingDirectory),
					std::move(declaredInput),
					std::move(declaredOutput));

				// No return value
				wrenEnsureSlots(_vm, 1);
				wrenSetSlotNull(_vm, 0);
			}
			catch(const std::exception& ex)
			{
				WrenHelpers::GenerateRuntimeError(_vm, ex.what());
			}
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
			if (moduleName == std::string_view(SoupModuleName))
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
			// Log::Diag("LoadModule: " + std::string(moduleName));

			WrenLoadModuleResult result = {0};
			result.onComplete = nullptr;

			// Inject Soup module
			if (moduleName == SoupModuleName)
			{
				result.source = GetSoupModuleSource();
			}
			else
			{
				// Attempt to load the module as a script file
				std::ifstream scriptFile(moduleName);
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
			"		if (!(declaredInput is List)) Fiber.abort(\"DeclaredInput must be a list.\")\n"
			"		if (!(declaredOutput is List)) Fiber.abort(\"DeclaredOutput must be a list.\")\n"
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
