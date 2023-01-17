#pragma once

#include "ExtensionTaskDetails.h"
#include "GenerateState.h"

namespace Soup::Core::Generate
{
	class GenerateHost : public WrenHost
	{
	private:
		static inline const char* SoupModuleName = "soup";
		static inline const char* SoupClassName = "Soup";
		static inline const char* SoupTaskClassName = "SoupTask";

	private:
		GenerateState* _state;

	public:
		GenerateHost(Path scriptFile, std::optional<Path> bundlesFile) :
			WrenHost(std::move(scriptFile), std::move(bundlesFile)),
			_state(nullptr)
		{
		}

		void SetState(GenerateState& state)
		{
			_state = &state;
		}

		std::vector<ExtensionTaskDetails> DiscoverExtensions()
		{
			auto extensions = std::vector<ExtensionTaskDetails>();

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
					auto classHandle = SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));
					if (WrenHelpers::HasParentType(_vm, classHandle, SoupTaskClassName))
					{
						Log::Diag("Found Build Task");
						auto className = WrenHelpers::GetClassName(_vm, classHandle);
						auto runBeforeList = CallRunBeforeGetter(classHandle);
						auto runAfterList = CallRunAfterGetter(classHandle);

						extensions.push_back(
							ExtensionTaskDetails(
								std::move(className),
								_scriptFile,
								std::move(runBeforeList),
								std::move(runAfterList)));
					}
				}
			}

			return extensions;
		}

		void EvaluateTask(const std::string& className)
		{
			// Load up the class
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, _scriptFile.ToString().c_str(), className.c_str(), 0);

			// Check if a class
			auto type = wrenGetSlotType(_vm, 0);
			if (type != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Extension class name was not a class");
			}

			auto classHandle = SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call Evaluate
			auto evaluateMethodHandle = SmartHandle(_vm, wrenMakeCallHandle(_vm, "evaluate()"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, evaluateMethodHandle));
		}

		ValueTable GetUpdatedActiveState()
		{
			Log::Diag("GetUpdatedActiveState");
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, SoupModuleName, SoupClassName, 0);

			auto soupClassType = wrenGetSlotType(_vm, 0);
			if (soupClassType != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Missing Class Soup");
			}

			auto soupClassHandle = SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call ActiveState
			auto activeStateGetterHandle = SmartHandle(_vm, wrenMakeCallHandle(_vm, "activeState"));
			wrenSetSlotHandle(_vm, 0, soupClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, activeStateGetterHandle));

			try
			{
				auto result = WrenValueTable::GetSlotTable(_vm, 0);
				return result;
			}
			catch(const InvalidTypeException& exception)
			{
				// Unwrap the type error
				auto stringBuilder = std::stringstream();
				stringBuilder << "Invalid type in ActiveState:" << exception.what();
				throw std::runtime_error(stringBuilder.str());
			}
		}

		ValueTable GetUpdatedSharedState()
		{
			Log::Diag("GetUpdatedSharedState");
			wrenEnsureSlots(_vm, 1);
			wrenGetVariable(_vm, SoupModuleName, SoupClassName, 0);

			auto soupClassType = wrenGetSlotType(_vm, 0);
			if (soupClassType != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Missing Class Soup");
			}

			auto soupClassHandle = SmartHandle(_vm, wrenGetSlotHandle(_vm, 0));

			// Call SharedState
			auto sharedStateGetterHandle = SmartHandle(_vm, wrenMakeCallHandle(_vm, "sharedState"));
			wrenSetSlotHandle(_vm, 0, soupClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, sharedStateGetterHandle));

			try
			{
				auto result = WrenValueTable::GetSlotTable(_vm, 0);
				return result;
			}
			catch(const InvalidTypeException& exception)
			{
				// Unwrap the type error
				auto stringBuilder = std::stringstream();
				stringBuilder << "Invalid type in SharedState:" << exception.what();
				throw std::runtime_error(stringBuilder.str());
			}
		}

	private:
		virtual bool IsBuiltInModule(std::string_view moduleName) override final
		{
			if (moduleName == std::string_view(SoupModuleName))
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
			// There is only one foreign method in the soup module.
			if (moduleName == SoupModuleName)
			{
				if (className == SoupClassName && isStatic)
				{
					if (signature == "loadGlobalState_()")
						return SoupLoadGlobalState;
					else if (signature == "loadActiveState_()")
						return SoupLoadActiveState;
					else if (signature == "loadSharedState_()")
						return SoupLoadSharedState;
					else if (signature == "createOperation_(_,_,_,_,_,_)")
						return SoupCreateOperation;
					else if (signature == "info_(_)")
						return SoupLogInfo;
					else if (signature == "warning_(_)")
						return SoupLogWarning;
					else if (signature == "error_(_)")
						return SoupLogError;
				}
			}

			return nullptr;
		}

		std::vector<std::string> CallRunBeforeGetter(WrenHandle* classHandle)
		{
			// Call RunBefore
			auto runBeforeGetterHandle = SmartHandle(_vm, wrenMakeCallHandle(_vm, "runBefore"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetSlotStringList(_vm, 0, 1);
		}

		std::vector<std::string> CallRunAfterGetter(WrenHandle* classHandle)
		{
			// Call RunAfter
			auto runBeforeGetterHandle = SmartHandle(_vm, wrenMakeCallHandle(_vm, "runAfter"));

			wrenSetSlotHandle(_vm, 0, classHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(_vm, runBeforeGetterHandle));

			return WrenHelpers::GetSlotStringList(_vm, 0, 1);
		}

		void SoupLoadGlobalState()
		{
			try
			{
				Log::Diag("SoupLoadGlobalState");
				if (_state == nullptr)
					throw std::runtime_error("Cannot load GlobalState at this time");

				WrenValueTable::SetSlotTable(_vm, 0, _state->GetGlobalState());
			}
			catch(const std::exception& ex)
			{
				WrenHelpers::GenerateRuntimeError(_vm, ex.what());
			}
		}

		void SoupLoadActiveState()
		{
			try
			{
				Log::Diag("SoupLoadActiveState");
				if (_state == nullptr)
					throw std::runtime_error("Cannot load ActiveState at this time");

				WrenValueTable::SetSlotTable(_vm, 0, _state->GetActiveState());
			}
			catch(const std::exception& exception)
			{
				WrenHelpers::GenerateRuntimeError(_vm, exception.what());
			}
		}

		void SoupLoadSharedState()
		{
			try
			{
				Log::Diag("SoupLoadSharedState");
				if (_state == nullptr)
					throw std::runtime_error("Cannot load SharedState at this time");

				WrenValueTable::SetSlotTable(_vm, 0, _state->GetSharedState());
			}
			catch(const std::exception& exception)
			{
				WrenHelpers::GenerateRuntimeError(_vm, exception.what());
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

		void SoupLogInfo()
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

		static void SoupLoadGlobalState(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLoadGlobalState();
		}

		static void SoupLoadActiveState(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLoadActiveState();
		}

		static void SoupLoadSharedState(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLoadSharedState();
		}

		static void SoupCreateOperation(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupCreateOperation();
		}

		static void SoupLogInfo(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLogInfo();
		}

		static void SoupLogWarning(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLogWarning();
		}

		static void SoupLogError(WrenVM* vm)
		{
			auto host = (GenerateHost*)wrenGetUserData(vm);
			host->SoupLogError();
		}

		static const char* GetSoupModuleSource()
		{
			return soupModuleSource;
		}

		static inline const char* soupModuleSource =
			"class SoupTask {\n"
			"	static runBefore { [] }\n"
			"	static runAfter { [] }\n"
			"	static evaluate() {}\n"
			"}\n"
			"\n"
			"class Soup {\n"
			"	static globalState {\n"
			"		if (__globalState is Null) __globalState = loadGlobalState_()\n"
			"		return __globalState\n"
			"	}\n"
			"\n"
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
			"	static info(message) {\n"
			"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
			"		info_(message)\n"
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
			"	foreign static loadGlobalState_()\n"
			"	foreign static loadActiveState_()\n"
			"	foreign static loadSharedState_()\n"
			"	foreign static createOperation_(title, executable, arguments, workingDirectory, declaredInput, declaredOutput)\n"
			"	foreign static info_(message)\n"
			"	foreign static warning_(message)\n"
			"	foreign static error_(message)\n"
			"}\n";
	};
}
