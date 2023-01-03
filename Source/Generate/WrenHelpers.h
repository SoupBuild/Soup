#pragma once

#include "Wren.hpp"

namespace WrenHelpers
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
}