// <copyright file="Main.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include <stdio.h>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include "wren.hpp"

static const char* soupModuleSource =
	"class Soup {\n"
	"	static debug(message) {\n"
	"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
	"		logMessage_(message, 1)\n"
	"	}\n"
	"\n"
	"	static warning(message) {\n"
	"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
	"		logMessage_(message, 2)\n"
	"	}\n"
	"\n"
	"	static error(message) {\n"
	"		if (!(message is String)) Fiber.abort(\"Message must be a string.\")\n"
	"		logMessage_(message, 3)\n"
	"	}\n"
	"\n"
	"	foreign static logMessage_(message, level)\n"
	"}\n";


void SoupLogMessage(WrenVM* vm)
{
	auto message = wrenGetSlotString(vm, 1);
	auto level = wrenGetSlotDouble(vm, 2);
	std::cout << "LogMessage: [" << level << "] " << message << std::endl;
}

const char* WrenSoupSource()
{
	return soupModuleSource;
}

WrenForeignMethodFn WrenBindForeignMethod(
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

	// There is only one foreign method in the soup module.
	if (classNameValue == "Soup" && isStatic)
	{
		if (signatureValue == "logMessage_(_,_)")
		{
			return SoupLogMessage;
		}
	}

	return nullptr;
}

static WrenLoadModuleResult LoadModule(WrenVM* vm, const char* module)
{
	(vm);
	auto moduleName = std::string_view(module);

	WrenLoadModuleResult result = {0};
	result.onComplete = nullptr;

	// Inject Soup module
	if (moduleName == "soup")
	{
		result.source = WrenSoupSource();
	}

	return result;
}

static void WriteCallback(WrenVM* vm, const char* text)
{
	(vm);
	printf("%s", text);
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
			std::cout << "Success!" << std::endl;
			break;
		}
	}
}

void Run()
{
	// Configure the Wren Virtual Machine
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.loadModuleFn = &LoadModule;
	config.bindForeignMethodFn = &WrenBindForeignMethod;
	config.writeFn = &WriteCallback;
	config.errorFn = &ErrorCallback;

	// Create the VM
	auto vm = wrenNewVM(&config);

	// Load the script
	std::ifstream scriptFile("Test.wren");
	auto script = std::string(
		std::istreambuf_iterator<char>(scriptFile),
		std::istreambuf_iterator<char>());

	// Interpret the script
	const char* module = "main";
	auto result = wrenInterpret(vm, module, script.c_str());
	ThrowIfFailed(result);

	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, module, "TestClass", 0);

	auto type = wrenGetSlotType(vm, 0);
	if (type != WREN_TYPE_UNKNOWN) {
		throw std::runtime_error("Missing Class TestClass");
	}

	auto testClassHandle = wrenGetSlotHandle(vm, 0);

	auto doStuffMethodHandle = wrenMakeCallHandle(vm, "DoStuff()");

	wrenSetSlotHandle(vm, 0, testClassHandle);
	result = wrenCall(vm, doStuffMethodHandle);
	ThrowIfFailed(result);

	wrenReleaseHandle(vm, doStuffMethodHandle);
	wrenReleaseHandle(vm, testClassHandle);

	wrenFreeVM(vm);
}

int main()
{
	try
	{
		Run();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
		return -1;
	}
}