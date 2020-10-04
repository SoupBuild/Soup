module;

#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

export module Soup.Cpp.Compiler.MSVC;

import Opal;
import Soup.Cpp.Compiler;

using namespace Opal;

#include "ArgumentBuilder.h"
#include "Compiler.h"
