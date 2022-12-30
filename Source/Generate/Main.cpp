//For more details, visit https://wren.io/embedding/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "wren.hpp"

static void writeFn(WrenVM* vm, const char* text)
{
	(vm);
	printf("%s", text);
}

void errorFn(
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

int main()
{
	WrenConfiguration config;
	wrenInitConfiguration(&config);
	config.writeFn = &writeFn;
	config.errorFn = &errorFn;
	WrenVM* vm = wrenNewVM(&config);

	const char* module = "main";
	
	std::ifstream scriptFile("Test.wren");
	auto script = std::string(
		std::istreambuf_iterator<char>(scriptFile),
		std::istreambuf_iterator<char>());

	WrenInterpretResult result = wrenInterpret(vm, module, script.c_str());

	switch (result)
	{
		case WREN_RESULT_COMPILE_ERROR:
		{
			printf("Compile Error!\n");
			break;
		}
		case WREN_RESULT_RUNTIME_ERROR:
		{
			printf("Runtime Error!\n");
			break;
		}
		case WREN_RESULT_SUCCESS:
		{
			printf("Success!\n");
			break;
		}
	}

	wrenFreeVM(vm);
}