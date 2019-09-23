
import std.core;

#include "LLVMBitCodeParser.h"

int main()
{
  try
  {
    auto stream = std::fstream(
      "D:/Repos/Soup/Source/Core/out/obj/Clang/Module.pcm",
      std::ios_base::in | std::ios_base::binary);
    Soup::LLVMBitCodeParser::Parse(stream);
  }
  catch (const std::exception& ex)
  {
    std::cout << std::string("Error: ") + ex.what() << std::endl;
    return -1;
  }
  catch (...)
  {
    std::cout << "Unknown Error!" << std::endl;
    return -1;
  }

  return 0;
}