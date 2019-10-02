
#include <iostream>

import TestWrapper;
import TestMultiVersion;


int main()
{
	TestWrapper_VersionNamespace::Wrapper wrapper;
	TestMultiVersion_VersionNamespace::MultiVersion multiVersion;

	std::wcout << 
		wrapper.GetValue() << L" " <<
		wrapper.GetVersion() << L" " <<
		multiVersion.GetVersion() << L" " <<
		multiVersion.GetVersionEx() << std::endl;

	return 0;
}
