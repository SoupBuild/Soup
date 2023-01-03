#pragma once

#include <fstream>
#include "WrenHost.h"

class GenerateEngine
{
public:
	static void Run()
	{
		auto vm = InitializeVM();

		// Load the script
		std::ifstream scriptFile("Test.wren");
		auto script = std::string(
			std::istreambuf_iterator<char>(scriptFile),
			std::istreambuf_iterator<char>());

		// Interpret the script
		WrenHelpers::ThrowIfFailed(wrenInterpret(vm, "main", script.c_str()));

		// Discover all class types
		wrenEnsureSlots(vm, 1);
		auto variableCount = wrenGetVariableCount(vm, "main");
		for (auto i = 0; i < variableCount; i++)
		{
			wrenGetVariableAt(vm, "main", i, 0);

			// Check if a class
			auto type = wrenGetSlotType(vm, 0);
			if (type == WREN_TYPE_UNKNOWN)
			{
				auto testClassHandle = WrenHelpers::SmartHandle(vm, wrenGetSlotHandle(vm, 0));
				if (WrenHelpers::HasParentType(vm, testClassHandle, "SoupExtension"))
				{
					std::cout << "Found Build Extension" << std::endl;
					EvaluateExtension(vm, testClassHandle);
				}
			}
		}

		wrenFreeVM(vm);
	}

private:
	static WrenVM* InitializeVM()
	{
		// Configure the Wren Virtual Machine
		WrenConfiguration config;
		wrenInitConfiguration(&config);
		config.loadModuleFn = &WrenHost::LoadModule;
		config.bindForeignMethodFn = &WrenHost::BindForeignMethod;
		config.writeFn = &WrenHost::WriteCallback;
		config.errorFn = &WrenHost::ErrorCallback;

		// Create the VM
		auto vm = wrenNewVM(&config);

		return vm;
	}

	static void EvaluateExtension(WrenVM* vm, WrenHandle* classHandle)
	{
		// Call Evaluate
		auto evaluateMethodHandle = WrenHelpers::SmartHandle(vm, wrenMakeCallHandle(vm, "evaluate()"));

		wrenSetSlotHandle(vm, 0, classHandle);
		WrenHelpers::ThrowIfFailed(wrenCall(vm, evaluateMethodHandle));

		wrenEnsureSlots(vm, 1);
		wrenGetVariable(vm, "soup", "Soup", 0);

		auto soupClassType = wrenGetSlotType(vm, 0);
		if (soupClassType != WREN_TYPE_UNKNOWN) {
			throw std::runtime_error("Missing Class Soup");
		}

		auto soupClassHandle = WrenHelpers::SmartHandle(vm, wrenGetSlotHandle(vm, 0));

		// Call ActiveState
		auto activeStateGetterHandle = WrenHelpers::SmartHandle(vm, wrenMakeCallHandle(vm, "activeState"));
		wrenSetSlotHandle(vm, 0, soupClassHandle);
		WrenHelpers::ThrowIfFailed(wrenCall(vm, activeStateGetterHandle));

		auto activeStateType = wrenGetSlotType(vm, 0);
		if (activeStateType != WREN_TYPE_MAP) {
			throw std::runtime_error("ActiveState must be a map");
		}

		wrenEnsureSlots(vm, 3);
		auto mapCount = wrenGetMapCount(vm, 0);
		std::cout << "ActiveState: " << mapCount << std::endl;
		for (auto i = 0; i < mapCount; i++)
		{
			wrenGetMapKeyValueAt(vm, 0, i, 1, 2);
			auto key = wrenGetSlotString(vm, 1);
			auto value = wrenGetSlotString(vm, 2);
			std::cout << key << " : " << value << std::endl;
		}
	}
};
