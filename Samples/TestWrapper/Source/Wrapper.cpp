module TestWrapper;
import TestMultiVersion;

using namespace TestWrapper_VersionNamespace;

Wrapper::Wrapper()
{
	_value = 100;
}

int Wrapper::GetValue()
{
	return _value;
}

int Wrapper::GetVersion()
{
	TestMultiVersion_VersionNamespace::MultiVersion version;
	return version.GetVersion();
}
