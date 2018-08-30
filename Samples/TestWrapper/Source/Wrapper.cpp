import TestWrapper;
import TestMultiVersion;

using namespace TestWrapper::SoupLatest;

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
  TestMultiVersion::v1_0_2::MultiVersion version;
  return version.GetVersion();
}
