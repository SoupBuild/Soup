#pragma once

#include "wren.hpp"
#include "WrenHelpers.h"
#include "SoupModule.h"

namespace Soup::Build::Generate::WrenHost
{
	WrenForeignMethodFn BindForeignMethod(
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
		result = SoupModule::TryBindForeignMethod(classNameValue, isStatic, signatureValue);
		if (result != nullptr)
			return result;

		return nullptr;
	}

	WrenLoadModuleResult LoadModule(WrenVM* vm, const char* module)
	{
		(vm);
		auto moduleName = std::string_view(module);

		WrenLoadModuleResult result = {0};
		result.onComplete = nullptr;

		// Inject Soup module
		if (moduleName == "soup")
			result.source = SoupModule::GetSource();

		return result;
	}

	void WriteCallback(WrenVM* vm, const char* text)
	{
		(vm);
		(text);
	}

	void ErrorCallback(
		WrenVM* vm,
		WrenErrorType errorType,
		const char* module,
		const int line,
		const char* msg)
	{
		(vm);
		switch (errorType)
		{
			case WREN_ERROR_COMPILE:
			{
				printf("[%s line %d] [Error] %s\n", module, line, msg);
				break;
			}
			case WREN_ERROR_STACK_TRACE:
			{
				printf("[%s line %d] in %s\n", module, line, msg);
				break;
			}
			case WREN_ERROR_RUNTIME:
			{
				printf("[Runtime Error] %s\n", msg);
				break;
			}
		}
	}
}
