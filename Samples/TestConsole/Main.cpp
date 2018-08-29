import TestWrapper;
import TestMultiVersion;
#include <iostream>

int main()
{
	TestWrapper::Wrapper wrapper;
	TestMultiVersion::MultiVersion multiVersion;

	std::wcout 
		<< wrapper.GetVersion() << L" " 
		<< multiVersion.GetVersion() << L" "
		<< multiVersion.GetVersionEx() << std::endl;

	return 0;
}