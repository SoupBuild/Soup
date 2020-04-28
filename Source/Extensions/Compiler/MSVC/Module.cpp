module;

#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

export module SoupCompilerMSVC;

import Opal;
import SoupCompiler;

using namespace Opal;

#include "ArgumentBuilder.h"
#include "Compiler.h"
