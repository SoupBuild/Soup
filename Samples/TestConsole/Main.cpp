import TestWrapper;
import TestMultiVersion;
#include <iostream>

int main()
{
	TestWrapper::v1_0_33::Wrapper wrapper;
	TestMultiVersion::v2_0_3::MultiVersion multiVersion;

	std::wcout 
		<< wrapper.GetValue() << L" " 
		<< wrapper.GetVersion() << L" " 
		<< multiVersion.GetVersion() << L" "
		<< multiVersion.GetVersionEx() << std::endl;

	return 0;
}
