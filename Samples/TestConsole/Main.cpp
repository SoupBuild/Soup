import TestWrapper;
import TestMultiVersion;

// Clang is broken withn iostream and modules...
#include <stdio.h>

int main()
{
	TestWrapper_VersionNamespace::Wrapper wrapper;
	TestMultiVersion_VersionNamespace::MultiVersion multiVersion;

	wprintf_s(
		L"%d %d %d %f",
		wrapper.GetValue(),
		wrapper.GetVersion(),
		multiVersion.GetVersion(),
		multiVersion.GetVersionEx());

	return 0;
}
