#include "MultiVersion.h"

namespace TestMultiVersion = TestMultiVersion_V1_1_1;
using namespace TestMultiVersion;

std::wstring MultiVersion::GetVersion()
{
  return L"Version1";
}