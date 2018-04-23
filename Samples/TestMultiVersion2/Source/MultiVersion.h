#pragma once
#include <string>

namespace TestMultiVersion
{
  class MultiVersion
  {
  public:
    std::wstring GetVersion();
    std::wstring GetVersionEx();
  };
}