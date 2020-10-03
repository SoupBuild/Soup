module;

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module Soup.Compiler.Clang;

import Opal;
import Soup.Compiler;
import Soup.Compiler.MSVC;

using namespace Opal;

#include "ArgumentBuilder.h"
#include "Compiler.h"
