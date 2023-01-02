#pragma once

#include <fstream>
#include "WrenHost.h"

class GenerateEngine
{
public:
	static void Run()
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

		// Load the script
		std::ifstream scriptFile("Test.wren");
		auto script = std::string(
			std::istreambuf_iterator<char>(scriptFile),
			std::istreambuf_iterator<char>());

		// Interpret the script
		WrenHelpers::ThrowIfFailed(wrenInterpret(vm, "main", script.c_str()));

		wrenEnsureSlots(vm, 1);
		wrenGetVariable(vm, "main", "TestClass", 0);

		auto type = wrenGetSlotType(vm, 0);
		if (type != WREN_TYPE_UNKNOWN) {
			throw std::runtime_error("Missing Class TestClass");
		}

		auto testClassHandle = wrenGetSlotHandle(vm, 0);

		if (WrenHelpers::HasParentType(vm, testClassHandle, "SoupExtension"))
		{
			// Call Evaluate
			auto evaluateMethodHandle = wrenMakeCallHandle(vm, "evaluate()");

			wrenSetSlotHandle(vm, 0, testClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(vm, evaluateMethodHandle));

			wrenReleaseHandle(vm, evaluateMethodHandle);

			wrenEnsureSlots(vm, 1);
			wrenGetVariable(vm, "soup", "Soup", 0);

			type = wrenGetSlotType(vm, 0);
			if (type != WREN_TYPE_UNKNOWN) {
				throw std::runtime_error("Missing Class Soup");
			}

			auto soupClassHandle = wrenGetSlotHandle(vm, 0);

			// Call ActiveState
			auto activeStateGetterHandle = wrenMakeCallHandle(vm, "activeState");
			wrenSetSlotHandle(vm, 0, soupClassHandle);
			WrenHelpers::ThrowIfFailed(wrenCall(vm, activeStateGetterHandle));

			type = wrenGetSlotType(vm, 0);
			if (type != WREN_TYPE_MAP) {
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

			wrenReleaseHandle(vm, activeStateGetterHandle);

			wrenReleaseHandle(vm, soupClassHandle);
		}

		wrenReleaseHandle(vm, testClassHandle);

		wrenFreeVM(vm);
	}
};
