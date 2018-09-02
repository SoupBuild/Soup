import TestWrapper;
import TestMultiVersion;
#include <iostream>

int main()
{
	TestWrapper_VersionNamespace::Wrapper wrapper;
	TestMultiVersion_VersionNamespace::MultiVersion multiVersion;

	std::wcout
		<< TestWrapper_VersionNamespace << L" "
		<< wrapper.GetValue() << L" "
		<< wrapper.GetVersion() << L" "
		<< multiVersion.GetVersion() << L" "
		<< multiVersion.GetVersionEx() << std::endl;

	return 0;
}
