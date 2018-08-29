import TestWrapper;
import TestMultiVersion;

using namespace TestWrapper;

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
  TestMultiVersion::MultiVersion version;
  return version.GetVersion();
}
