#pragma warning(push, 3)
#include "gravity_compiler.h"
#include "gravity_macros.h"
#include "gravity_core.h"
#include "gravity_vm.h"
#pragma warning(pop)

const char *source_code = " \
func sum (a, b) {   \
    return a + b    \
}   \
    \
func mul (a, b) {   \
    return a * b    \
}   \
    \
func main () {   \
    var a = 10   \
    var b = 20   \
    return sum(a, b) + mul(a, b)   \
}   \
";

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

int main ()
{
	// setup a delegate struct (much more options are available)
	gravity_delegate_t delegate = {.error_callback = report_error};

	// allocate a new compiler
	gravity_compiler_t *compiler = gravity_compiler_create(&delegate);

	// compile Gravity source code into a closure (bytecode)
	gravity_closure_t *closure = gravity_compiler_run(compiler, source_code, strlen(source_code), 0, true, true);

	// allocate a new Gravity VM
	gravity_vm *vm = gravity_vm_new(&delegate);

	// transfer memory from the compiler (front-end) to the VM (back-end)
	gravity_compiler_transfer(compiler, vm);

	// once the memory has been transferred, you can get rid of the front-end
	gravity_compiler_free(compiler);

	// execute main closure
	if (gravity_vm_runmain(vm, closure)) {
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