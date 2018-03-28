#include <TestProject1.h>
#include <TestMultiVersion.h>
#include <iostream>

int main()
{
	TestProject1::Wrapper wrapper;
	TestMultiVersion::MultiVersion multiVersion;

	std::wcout << wrapper.GetVersion() << L" " << multiVersion.GetVersionEx() << std::endl;

	return 0;
}