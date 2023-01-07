#pragma once

#include "Wren.hpp"

namespace Soup::Core::Generate::WrenHelpers
{
	class SmartHandle
	{
	private:
		WrenVM* _vm;
		WrenHandle* _handle;

	public:
		SmartHandle(WrenVM* vm, WrenHandle* handle)
		{
			if (vm == nullptr)
				throw std::runtime_error("VM cannot be null");
			if (handle == nullptr)
				throw std::runtime_error("Handle cannot be null");

			_vm = vm;
			_handle = handle;
		}

		~SmartHandle()
		{
			wrenReleaseHandle(_vm, _handle);
			_handle = nullptr;
		}

		operator WrenHandle*() const
		{
			return _handle;
		}
	};

	void ThrowIfFailed(WrenInterpretResult result)
	{
		switch (result)
		{
			case WREN_RESULT_COMPILE_ERROR:
			{
				throw std::runtime_error("Compile Error!");
			}
			case WREN_RESULT_RUNTIME_ERROR:
			{
				throw std::runtime_error( "Runtime Error!");
			}
			case WREN_RESULT_SUCCESS:
			{
				break;
			}
		}
	}

	void GenerateRuntimeError(WrenVM* vm, std::string_view message)
	{
		wrenEnsureSlots(vm, 1);
		wrenSetSlotString(vm, 0, message.data());
		wrenAbortFiber(vm, 0);
	}

	bool HasParentType(WrenVM* vm, WrenHandle* classHandle, std::string_view parentClassType)
	{
		wrenSetSlotHandle(vm, 0, classHandle);

		// Create call handle for supertype and toString methods
		auto supertypeHandle = WrenHelpers::SmartHandle(vm, wrenMakeCallHandle(vm, "supertype"));
		auto toStringHandle = WrenHelpers::SmartHandle(vm, wrenMakeCallHandle(vm, "toString"));

		// Call the supertype method
		ThrowIfFailed(wrenCall(vm, supertypeHandle));

		// This returns the class reference for the supertype in slot 0.
		// Now call its toString method
		ThrowIfFailed(wrenCall(vm, toStringHandle));

		// Retrieve the return value (a string) from slot 0.
		auto parentType = std::string_view(wrenGetSlotString(vm, 0));

		return parentClassType == parentType;
	}

	std::string GetClassName(WrenVM* vm, WrenHandle* classHandle)
	{
		wrenSetSlotHandle(vm, 0, classHandle);

		// Create call handle for name method
		auto nameHandle = WrenHelpers::SmartHandle(vm, wrenMakeCallHandle(vm, "name"));

		// Call the name method
		ThrowIfFailed(wrenCall(vm, nameHandle));

		// Retrieve the return value (a string) from slot 0.
		auto type = std::string(wrenGetSlotString(vm, 0));

		return type;
	}

	std::vector<std::string> GetResultAsStringList(WrenVM* vm)
	{
		auto responseType = wrenGetSlotType(vm, 0);
		if (responseType != WREN_TYPE_LIST) {
			throw std::runtime_error("RunBefore did not return a list");
		}

		auto listCount = wrenGetListCount(vm, 0);

		// Load each element into the second slot
		auto result = std::vector<std::string>();
		wrenEnsureSlots(vm, 2);
		for (auto i = 0; i < listCount; i++)
		{
			wrenGetListElement(vm, 0, i, 1);
			
			auto elementType = wrenGetSlotType(vm, 1);
			if (elementType != WREN_TYPE_STRING) {
				throw std::runtime_error("Element in list must be string");
			}

			auto element = std::string(wrenGetSlotString(vm, 1));
			result.push_back(std::move(element));
		}

		return result;
	}
}