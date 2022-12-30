#pragma warning(push, 3)
#include "gravity_compiler.h"
#include "gravity_macros.h"
#include "gravity_core.h"
#include "gravity_vm.h"
#pragma warning(pop)

#include <string>
#include <fstream>
#include <streambuf>

// a very simple report error callback function
void report_error(
	gravity_vm *vm,
	error_type_t error_type,
	const char *message,
	error_desc_t error_desc,
	void *xdata)
{
	(vm);
	(error_type);
	(error_desc);
	(xdata);
	printf("%s\n", message);
	exit(0);
}

int main()
{
	std::ifstream t("Test.gravity");
	auto source_code = std::string(
		std::istreambuf_iterator<char>(t),
		std::istreambuf_iterator<char>());

	// setup a delegate struct
	gravity_delegate_t delegate = {.error_callback = report_error};

	// allocate a new compiler
	gravity_compiler_t *compiler = gravity_compiler_create(&delegate);

	// compile Gravity source code into bytecode
	gravity_closure_t *closure = gravity_compiler_run(
		compiler,
		source_code.c_str(),
		source_code.size(),
		0,
		true,
		true);

	// allocate a new Gravity VM
	gravity_vm *vm = gravity_vm_new(&delegate);

	// transfer memory from the compiler (front-end) to the VM (back-end)
	gravity_compiler_transfer(compiler, vm);

	// once the memory has been transferred, you can get rid of the front-end
	gravity_compiler_free(compiler);

	// load closure into VM
	gravity_vm_loadclosure(vm, closure);

	// lookup a reference to the mul closure into the Gravity VM
	gravity_value_t mul_function = gravity_vm_getvalue(vm, "mul", (uint32_t)strlen("mul"));
	if (!VALUE_ISA_CLOSURE(mul_function)) {
		printf("Unable to find mul function into Gravity VM.\n");
		return -1;
	}

	// convert function to closure
	gravity_closure_t *mul_closure = VALUE_AS_CLOSURE(mul_function);

	// prepare parameters
	gravity_value_t p1 = gravity_value_from_int(30);
	gravity_value_t p2 = gravity_value_from_int(40);
	gravity_value_t params[] = { p1, p2 };

	// execute mul closure
	if (gravity_vm_runclosure(vm, mul_closure, gravity_value_from_null(), params, 2))
	{
		// retrieve returned result
		gravity_value_t result = gravity_vm_result(vm);

		// dump result to a C string and print it to stdout
		char buffer[512];
		gravity_value_dump(vm, result, buffer, sizeof(buffer));
		printf("RESULT: %s\n", buffer);
	}

	// free VM and core libraries (implicitly allocated by the VM)
	gravity_vm_free(vm);
	gravity_core_free();

	return 0;
}