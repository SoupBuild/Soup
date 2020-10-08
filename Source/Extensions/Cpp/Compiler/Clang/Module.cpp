module;

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module Soup.Cpp.Compiler.Clang;

import Opal;
import Soup.Cpp.Compiler;
import Soup.Cpp.Compiler.MSVC;

using namespace Opal;

#include "Compiler.h"
