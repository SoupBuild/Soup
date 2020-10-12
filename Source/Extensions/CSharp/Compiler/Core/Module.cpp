module;

#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

export module Soup.CSharp.Compiler;

import Opal;
import Soup.Build.Utilities;

using namespace Opal;

#include "Compiler.h"
#include "CompileArguments.h"

#include "BuildEngine.h"
