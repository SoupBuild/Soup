#include "MultiVersion.h"

using namespace TestMultiVersion;

std::wstring MultiVersion::GetVersion()
{
  return L"Version1.2";
}

std::wstring MultiVersion::GetVersionEx()
{
  return L"Version2";
}