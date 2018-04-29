#pragma once
#include <string>

namespace TestMultiVersion
{
SOUP_PKG_ACTIVE namespace SOUP_PKG_VERSION
{
  class MultiVersion
  {
  public:
    std::wstring GetVersion();
    std::wstring GetVersionEx();
  };
}
}