#pragma once

class SoupModule
{
private:
	static inline const char* soupModuleSource =
		"class SoupExtension {\n"
		"	static runBefore { [] }\n"
		"	static runAfter { [] }\n"
		"	static evaluate() {}\n"
		"}\n"
		"\n"
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

	static void SoupLoadActiveState(WrenVM* vm)
	{
		Log::Diag("SoupLoadActiveState");
		wrenSetSlotNewMap(vm, 0);
	}

	static void SoupLoadSharedState(WrenVM* vm)
	{
		Log::Diag("SoupLoadSharedState");
		wrenSetSlotNewMap(vm, 0);
	}

	static void SoupCreateOperation(WrenVM* vm)
	{
		Log::Diag("SoupCreateOperation");
		wrenSetSlotNull(vm, 0);
	}

	static void SoupLogDebug(WrenVM* vm)
	{
		auto message = wrenGetSlotString(vm, 1);
		Log::Info(message);
		wrenSetSlotNull(vm, 0);
	}

	static void SoupLogWarning(WrenVM* vm)
	{
		auto message = wrenGetSlotString(vm, 1);
		Log::Warning(message);
		wrenSetSlotNull(vm, 0);
	}

	static void SoupLogError(WrenVM* vm)
	{
		auto message = wrenGetSlotString(vm, 1);
		Log::Error(message);
		wrenSetSlotNull(vm, 0);
	}

public:
	static const char* GetSource()
	{
		return soupModuleSource;
	}

	static WrenForeignMethodFn TryBindForeignMethod(
		std::string_view className,
		bool isStatic,
		std::string_view signature)
	{
		// There is only one foreign method in the soup module.
		if (className == "Soup" && isStatic)
		{
			if (signature == "loadActiveState_()")
				return SoupLoadActiveState;
			else if (signature == "loadSharedState_()")
				return SoupLoadSharedState;
			else if (signature == "createOperation_(_,_,_,_,_,_)")
				return SoupCreateOperation;
			else if (signature == "debug_(_)")
				return SoupLogDebug;
			else if (signature == "warning_(_)")
				return SoupLogWarning;
			else if (signature == "error_(_)")
				return SoupLogError;
		}

		return nullptr;
	}
};