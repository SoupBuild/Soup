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

void SoupLoadActiveState(WrenVM* vm)
{
	std::cout << "SoupLoadActiveState: " << std::endl;
	wrenSetSlotNewMap(vm, 0);
}

void SoupLoadSharedState(WrenVM* vm)
{
	std::cout << "SoupLoadSharedState: " << std::endl;
	wrenSetSlotNewMap(vm, 0);
}

void SoupCreateOperation(WrenVM* vm)
{
	std::cout << "SoupCreateOperation: " << std::endl;
	wrenSetSlotNull(vm, 0);
}

void SoupLogDebug(WrenVM* vm)
{
	auto message = wrenGetSlotString(vm, 1);
	std::cout << "LogMessage: " << message << std::endl;
	wrenSetSlotNull(vm, 0);
}

void SoupLogWarning(WrenVM* vm)
{
	auto message = wrenGetSlotString(vm, 1);
	std::cout << "LogMessage: " << message << std::endl;
	wrenSetSlotNull(vm, 0);
}

void SoupLogError(WrenVM* vm)
{
	auto message = wrenGetSlotString(vm, 1);
	std::cout << "LogMessage: " << message << std::endl;
	wrenSetSlotNull(vm, 0);
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
		if (signatureValue == "loadActiveState_()")
			return SoupLoadActiveState;
		else if (signatureValue == "loadSharedState_()")
			return SoupLoadSharedState;
		else if (signatureValue == "createOperation_(_,_,_,_,_,_)")
			return SoupCreateOperation;
		else if (signatureValue == "debug_(_)")
			return SoupLogDebug;
		else if (signatureValue == "warning_(_)")
			return SoupLogWarning;
		else if (signatureValue == "error_(_)")
			return SoupLogError;
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
		result.source = WrenSoupSource();

	return result;
}

static void WriteCallback(WrenVM* vm, const char* text)
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
	auto result = wrenInterpret(vm, "main", script.c_str());
	ThrowIfFailed(result);

	wrenEnsureSlots(vm, 1);
	wrenGetVariable(vm, "main", "TestClass", 0);

	auto type = wrenGetSlotType(vm, 0);
	if (type != WREN_TYPE_UNKNOWN) {
		throw std::runtime_error("Missing Class TestClass");
	}

	auto testClassHandle = wrenGetSlotHandle(vm, 0);

	// Call DoStuff
	auto doStuffMethodHandle = wrenMakeCallHandle(vm, "doStuff()");
	wrenSetSlotHandle(vm, 0, testClassHandle);
	result = wrenCall(vm, doStuffMethodHandle);
	ThrowIfFailed(result);
	wrenReleaseHandle(vm, doStuffMethodHandle);

	wrenReleaseHandle(vm, testClassHandle);

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
	result = wrenCall(vm, activeStateGetterHandle);
	ThrowIfFailed(result);

	type = wrenGetSlotType(vm, 0);
	if (type != WREN_TYPE_MAP) {
		throw std::runtime_error("ActiveState must be a map");
	}

	wrenEnsureSlots(vm, 3);
	auto mapCount = wrenGetMapCount(vm, 0);
	std::cout << "ActiveState: " << mapCount << std::endl;
	for (auto i = 0; i < mapCount; i++)
	{
		wrenGetMapKeyValue(vm, 0, i, 1, 2);
		auto key = wrenGetSlotString(vm, 1);
		auto value = wrenGetSlotString(vm, 2);
		std::cout << key << " : " << value << std::endl;
	}

	wrenReleaseHandle(vm, activeStateGetterHandle);

	wrenReleaseHandle(vm, soupClassHandle);

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