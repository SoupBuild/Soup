module;

#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

export module Soup.Compiler.MSVC;

import Opal;
import Soup.Compiler;

using namespace Opal;

#include "ArgumentBuilder.h"
#include "Compiler.h"
