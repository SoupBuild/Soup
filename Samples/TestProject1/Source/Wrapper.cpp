#include "Wrapper.h"
#include <TestMultiVersion.h>

using namespace TestProject1;

Wrapper::Wrapper()
{
  _value = 1;
}

int Wrapper::GetValue()
{
  return _value;
}

std::wstring Wrapper::GetVersion()
{
  TestMultiVersion::MultiVersion version;
  return version.GetVersion();
}