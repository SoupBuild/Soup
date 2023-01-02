#pragma once

#include "Wren.hpp"

class WrenHelpers
{
public:
	static void ThrowIfFailed(WrenInterpretResult result)
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

	static bool HasParentType(WrenVM* vm, WrenHandle* classHandle, std::string_view parentClassType)
	{
		wrenSetSlotHandle(vm, 0, classHandle);

		// Create call handle for supertype method
		auto supertypeHandle = wrenMakeCallHandle(vm, "supertype");
		// Create call handle for toString method
		auto toStringHandle = wrenMakeCallHandle(vm, "toString");

		// Call the supertype method
		ThrowIfFailed(wrenCall(vm, supertypeHandle));

		// This returns the class reference for the supertype in slot 0.
		// Now call its toString method
		ThrowIfFailed(wrenCall(vm, toStringHandle));

		// Retrieve the return value (a string) from slot 0.
		auto parentType = std::string_view(wrenGetSlotString(vm, 0));
		std::cout << "ParentType: " << parentType << std::endl;

		wrenReleaseHandle(vm, supertypeHandle);
		wrenReleaseHandle(vm, toStringHandle);

		return parentClassType == parentType;
	}
};